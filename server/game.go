package main

import (
	"errors"
	"fmt"
	"math/rand"
	"strconv"
	"strings"
	"time"

	"github.com/google/uuid"
	"github.com/sirupsen/logrus"
)

const (
	maxPlayers = 5
)

type game struct {
	state
	l       *lobby
	id      string
	name    string
	mailbox chan<- eventer
	done    <-chan struct{}
	log     *logrus.Entry
}

func newGame(l *lobby, name string) (*game, error) {
	mailbox := make(chan eventer, 64)
	done := make(chan struct{})
	log := logrus.WithFields(logrus.Fields{
		"room": name,
		"file": "game.go",
	})

	id, err := uuid.NewRandom()
	if err != nil {
		return nil, err
	}

	game := game{
		l:       l,
		id:      id.String(),
		name:    name,
		mailbox: mailbox,
		done:    done,
		log:     log,
	}
	go game.loop(mailbox, done)
	l.add(&game)
	return &game, nil
}

func (g *game) join(p Entity) error {
	errChan := make(chan error)
	e := &eventJoin{&event{p}, errChan}
	g.mailbox <- e
	err := <-errChan
	if err != nil {
		return err
	}

	p.Loop()
	return nil
}

// info gives the game's detail in the following format:
// <name>,<num_players>,<started>
func (g *game) info() string {
	count := 0
	for _, p := range g.players {
		if _, ok := p.(*human); ok {
			count++
		}
	}

	players := strconv.Itoa(count)
	return g.name + "," + players
}

func (g *game) broadcast(v interface{}) {
	dones := make([]<-chan error, 0, len(g.players))
	for _, p := range g.players {
		dones = append(dones, send(p, v))
	}

	timeout := 10 * time.Second
	var err error
	for i, p := range g.players {
		select {
		case err = <-dones[i]:
		case <-time.After(timeout):
			err = fmt.Errorf("%s doesn't receive in %d seconds", p.Name(), timeout)
		}

		if err != nil {
			// TODO! Remove player from the game
		}
	}
}

func (g *game) loop(mailbox <-chan eventer, done chan<- struct{}) {
	defer g.log.Info("game has been closed")
	defer close(done)

	for {
		e := <-mailbox
		changed := g.handle(e)

		if !changed {
			continue
		}

		if g.finished {
			g.broadcast(&respFinish{})
			break
		}
		g.broadcast(g.state.encode()) // update players with current state
	}
}

func (g *game) handle(e eventer) (changed bool) {
	before := g.state
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			from := e.From()
			g.log.WithField("player", from.Name()).Error(reason)
			send(from, respError{Reason: reason})
			// rollback state
			g.state = before
			changed = false
		}
	}()

	switch v := e.(type) {
	case *eventBet:
		g.handleBet(v)
	case *eventLeave:
		g.handleLeave(v)
	case *eventJoin:
		g.handleJoin(v)
	case *eventStart:
		g.handleStart(v)
	case *eventCall:
		g.handleCall(v)
	default:
		panic("invalid command")
	}

	return true
}

// randomize existing dices
func (g *game) roll() {
	// TODO! probably replace this with struct instead
	resp := make(map[string]string)
	resp["type"] = "roll"
	for idx, p := range g.players {
		dice := p.Dice()
		diceStr := make([]string, 0, len(dice))

		for i := 0; i < len(dice); i++ {
			dice[i] = rand.Int() % 6
			diceStr = append(diceStr, strconv.Itoa(dice[i]))
		}

		idx := strconv.Itoa(idx)
		encoded := strings.Join(diceStr, ",")
		resp[idx] = encoded
	}
	g.broadcast(resp)
}

func (g *game) handleBet(e *eventBet) {
	g.lastBet.quantity = e.quantity
	g.lastBet.face = e.face
	g.turn = (g.turn + 1) % len(g.players)
	for {
		g.turn = (g.turn + 1) % len(g.players)
		if len(g.players[g.turn].Dice()) != 0 {
			break
		}
	}
	g.round++
	g.calledLiar = false
}

func (g *game) handleLeave(e *eventLeave) {
	humanCount := 0
	for _, p := range g.players {
		_, ok := p.(*human)
		if ok {
			humanCount++
		}
	}
	if humanCount == 1 {
		g.finished = true
		g.l.close(g)
		return
	}

	idx := -1
	for i, p := range g.players {
		if p == e.From() {
			idx = i
			break
		}
	}

	if idx == -1 {
		return
	}

	g.log.Info(e.From().Name(), " leaving because ", e.reason)
	p := g.players[idx].(*human)
	bot := newBotFromHuman(p)
	g.players[idx] = bot
	g.log.Info("replacing ", p.Name(), " with a bot")
	if g.started {
		go bot.Loop()
	}

	g.l.update(g)
}

func (g *game) handleJoin(e *eventJoin) {
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			e.errChan <- errors.New(reason)
			panic(reason) // delegates to root handler for logging, etc
		}
		e.errChan <- nil
	}()

	if len(g.players) >= maxPlayers {
		panic("game is already full")
	}

	if g.started {
		panic("game has started")
	}

	from := e.From()

	// find a valid id here
	id := len(g.players)
	for i, p := range g.players {
		if _, ok := p.(*bot); ok {
			id = i
			break
		}
	}
	from.SetID(id)

	var err error
	timeout := 2 * time.Second
	select {
	case err = <-send(from, &respJoin{Index: id}):
	case <-time.After(timeout):
		err = fmt.Errorf("%s failed to receive in %d seconds", from.Name(), timeout)
	}

	if err != nil {
		panic(err.Error())
	}

	g.log.Info(from.Name(), " joined")
	if id < len(g.players) {
		g.players[id] = from
	} else {
		g.players = append(g.players, from)
		g.l.update(g)
	}
}

func (g *game) handleStart(e *eventStart) {
	// fill up players with bot and start existing bots
	for _, p := range g.players {
		b, ok := p.(*bot)
		if !ok {
			continue
		}
		go b.Loop()
	}

	need := maxPlayers - len(g.players)
	for i := 0; i < need; i++ {
		b := newRandomBot(g)
		go b.Loop()
		g.players = append(g.players, b)
	}

	g.started = true
	g.calledLiar = true // set this true, so that nobody can call liar
	resp := respStart{}
	g.broadcast(&resp)
	g.roll()
	g.l.close(g)
}

func (g *game) handleCall(e *eventCall) {
	if g.calledLiar {
		panic("somebody has called liar in last turn")
	}

	g.broadcast(&respCall{})

	sums := make([]int, 6)
	for _, p := range g.players {
		dice := p.Dice()
		for _, die := range dice {
			sums[die]++
		}
	}

	if sums[g.lastBet.face] < g.lastBet.quantity {
		g.lastPlayer.LoseDie()
	} else {
		e.From().LoseDie()
	}

	// TODO! probably add some delays between on call and roll

	g.roll()
	g.lastBet = bet{quantity: 0, face: 1}
	g.numDices--
	g.calledLiar = true
	if g.numDices == 0 {
		g.finished = true
	}
}
