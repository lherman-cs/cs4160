package main

import (
	"errors"
	"fmt"
	"io"
	"strconv"
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

func (g *game) join(conn io.ReadWriter) error {
	h := newHuman(g, conn)
	errChan := make(chan error)
	e := &eventJoin{&event{h}, errChan}
	g.mailbox <- e
	err := <-errChan
	if err != nil {
		return err
	}

	h.loop()
	return nil
}

// info gives the game's detail in the following format:
// <name>,<num_players>,<started>
func (g *game) info() string {
	players := strconv.Itoa(len(g.players))
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
			err = fmt.Errorf("%s doesn't receive in %d seconds", p.name, timeout)
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

		g.broadcast(g.state.encode()) // update players with current state
		if g.finished {
			break
		}
	}
}

func (g *game) handle(e eventer) (changed bool) {
	before := g.state
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			g.log.WithField("player", e.From().name).Error(reason)
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
	default:
		panic("invalid command")
	}

	return true
}

func (g *game) handleBet(e *eventBet) {
	g.lastBet.quantity = e.quantity
	g.lastBet.face = e.face
	g.turn = (g.turn + 1) % len(g.players)
	g.round++
}

func (g *game) handleLeave(e *eventLeave) {
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

	g.players = append(g.players[:idx], g.players[idx+1:]...)
	g.log.Info(e.From().name, " leaving because ", e.reason)

	if len(g.players) == 0 {
		g.finished = true
		g.l.close(g)
		return
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
	var err error
	timeout := 2 * time.Second
	select {
	case err = <-send(from, &respJoin{Index: len(g.players)}):
	case <-time.After(timeout):
		err = fmt.Errorf("%s failed to receive in %d seconds", from.name, timeout)
	}

	if err != nil {
		panic(err.Error())
	}

	g.players = append(g.players, from)
	g.log.Info(from.name, " joined")
	g.l.update(g)
}

func (g *game) handleStart(e *eventStart) {
	g.started = true
	resp := respStart{}
	g.broadcast(&resp)
	g.l.close(g)
}
