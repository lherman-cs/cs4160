package main

import (
	"fmt"
	"io"
	"sync"
)

const (
	maxPlayers = 5
)

type game struct {
	name    string
	players []*human
	m       sync.Mutex
}

func newGame(name string) *game {
	return &game{
		name: name,
	}
}

func (g *game) join(conn io.ReadWriter) error {
	g.m.Lock()
	defer g.m.Unlock()

	if len(g.players) == maxPlayers {
		return fmt.Errorf("game is already full")
	}

	g.players = append(g.players, newHuman(conn))
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
