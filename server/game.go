package main

import (
	"fmt"
	"io"
	"sync"

	log "github.com/sirupsen/logrus"
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

	g.players = append(g.players, newHuman(conn, g.mailbox))
	newEncoder(conn).encode(map[string]string{})
	g.m.Unlock()
	<-g.done
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
	for {
		msg := <-mailbox
		log.Info(msg)
	}
}
