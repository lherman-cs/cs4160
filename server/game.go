package main

import (
	"fmt"
	"io"
	"sync"

	"github.com/Pallinder/go-randomdata"
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

func (r *game) join(conn io.ReadWriter) error {
	r.m.Lock()
	defer r.m.Unlock()

	if len(r.players) == maxPlayers {
		return fmt.Errorf("game is already full")
	}

	human := newHuman(conn, randomdata.SillyName())
	r.players = append(r.players, human)
	return nil
}

// joinedPlayers get names who have joined
func (r *game) joinedPlayers() []string {
	r.m.Lock()
	defer r.m.Unlock()

	players := make([]string, 0, len(r.players))
	for _, p := range r.players {
		players = append(players, p.name)
	}

	return players
}
