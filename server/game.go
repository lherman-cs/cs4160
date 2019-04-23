package main

import (
	"fmt"
	"io"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/sirupsen/logrus"
)

const (
	maxPlayers = 5
)

type game struct {
	state
	name    string
	mailbox chan<- map[string]string
	done    <-chan struct{}
	log     *logrus.Entry
	m       sync.Mutex
}

func newGame(name string) *game {
	mailbox := make(chan map[string]string, 64)
	done := make(chan struct{})
	log := logrus.WithFields(logrus.Fields{
		"room": name,
	})
	game := game{
		name:    name,
		mailbox: mailbox,
		done:    done,
		log:     log,
	}
	go game.loop(mailbox, done)

	return &game
}

func (g *game) join(conn io.ReadWriter) error {
	g.m.Lock()
	if len(g.players) == maxPlayers {
		g.m.Unlock()
		return fmt.Errorf("game is already full")
	}

	h := newHuman(g, conn)
	g.players = append(g.players, h)
	g.log.Info(h.name, " joined")
	newEncoder(conn).encode(map[string]string{})
	g.m.Unlock()
	h.loop()
	return nil
}

// joinedPlayers get names who have joined
func (g *game) joinedPlayers() []string {
	g.m.Lock()
	defer g.m.Unlock()

	players := make([]string, 0, len(g.players))
	for _, p := range g.players {
		players = append(players, p.name)
	}

	return players
}

func (g *game) loop(mailbox <-chan map[string]string, done chan<- struct{}) {
	defer close(done)

	broadcast := func() {
		currentState := g.encode()

		dones := make([]chan struct{}, len(g.players))
		for i, p := range g.players {
			dones[i] = make(chan struct{})
			go func(done chan<- struct{}, p *human) {
				newEncoder(p.conn).encode(currentState)
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
		<-mailbox // TODO! handle msg

		broadcast() // update players with current state
	}
}

type status uint8

type state struct {
	players  []*human
	turn     int
	round    int
	numDices int
}

func (s *state) encode() map[string]string {
	players := make([]string, 0, len(s.players))
	for _, p := range s.players {
		players = append(players, p.name)
	}

	return map[string]string{
		"players":   strings.Join(players, ","),
		"turn":      strconv.Itoa(s.turn),
		"round":     strconv.Itoa(s.round),
		"num_dices": strconv.Itoa(s.numDices),
	}
}
