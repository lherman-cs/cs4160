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

type room struct {
	name    string
	players []*human
	m       sync.Mutex
}

func newRoom(name string) *room {
	return &room{
		name: name,
	}
}

func (r *room) join(conn io.ReadWriter) error {
	r.m.Lock()
	defer r.m.Unlock()

	if len(r.players) == maxPlayers {
		return fmt.Errorf("room is already full")
	}

	human := newHuman(conn, randomdata.SillyName())
	r.players = append(r.players, human)
	return nil
}

// joinedPlayers get names who have joined
func (r *room) joinedPlayers() []string {
	r.m.Lock()
	defer r.m.Unlock()

	players := make([]string, 0, len(r.players))
	for _, p := range r.players {
		players = append(players, p.name)
	}

	return players
}
