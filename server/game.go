package main

import (
	"fmt"
	"io"
	"sync"
	"time"
)

const (
	maxPlayers = 5
)

type game struct {
	name    string
	players []*human
	mailbox chan<- map[string]string
	done    <-chan struct{}
	m       sync.Mutex
}

func newGame(name string) *game {
	mailbox := make(chan map[string]string, 64)
	done := make(chan struct{})
	game := game{
		name:    name,
		mailbox: mailbox,
		done:    done,
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

	broadcast := func(resp map[string]string) {
		dones := make([]chan struct{}, len(g.players))
		for i, p := range g.players {
			dones[i] = make(chan struct{})
			go func(done chan<- struct{}, p *human) {
				newEncoder(p.conn).encode(resp)
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
		msg := <-mailbox

		// broadcast
		broadcast(msg) // TODO! this is just echoing
	}
}
