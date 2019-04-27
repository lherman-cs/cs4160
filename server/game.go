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
	id      string
	name    string
	mailbox chan<- eventer
	done    <-chan struct{}
	log     *logrus.Entry
}

func newGame(name string) (*game, error) {
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
		id:      id.String(),
		name:    name,
		mailbox: mailbox,
		done:    done,
		log:     log,
	}
	go game.loop(mailbox, done)
	mainLobby.add(&game)
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
// <name>,<num_players>
func (g *game) info() string {
	return g.name + "," + strconv.Itoa(len(g.players))
}

func (g *game) loop(mailbox <-chan eventer, done chan<- struct{}) {
	defer close(done)

	broadcast := func() {
		currentState := g.encode()

		dones := make([]chan struct{}, len(g.players))
		for i, p := range g.players {
			dones[i] = make(chan struct{})
			go func(done chan<- struct{}, p *human) {
				newEncoder(p).encode(currentState)
				close(done)
			}(dones[i], p)
		}

		for i := range g.players {
			select {
			case <-dones[i]:
			case <-time.After(time.Second * 10):
				// TODO! Remove player from the game
			}
		}
	}

	for {
		e := <-mailbox
		changed := g.handle(e)

		if changed {
			broadcast() // update players with current state
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
	default:
		panic("invalid command")
	}

	return true
}

func (g *game) handleBet(e *eventBet) {
	g.lastBet.quantity = e.quantity
	g.lastBet.face = e.face
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
		g.log.Info("Nobody is in the room. Closing...")
		mainLobby.close(g)
		return
	}
	mainLobby.update(g)
}

func (g *game) handleJoin(e *eventJoin) {
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			g.log.Info(reason)
			e.errChan <- errors.New(reason)
			return
		}
		e.errChan <- nil
	}()

	if len(g.players) >= maxPlayers {
		panic("game is already full")
	}
	from := e.From()
	var err error
	timeout := 2 * time.Second
	select {
	case err = <-send(from, &respJoin{Index: strconv.Itoa(len(g.players))}):
	case <-time.After(timeout):
		err = fmt.Errorf("%s failed to receive in %d seconds", from.name, timeout)
	}

	if err != nil {
		panic(err.Error())
	}

	g.players = append(g.players, from)
	g.log.Info(from.name, " joined")
	mainLobby.update(g)
}
