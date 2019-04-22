package main

import (
	"net"
	"sync"
)

type room struct {
	name    string
	players []net.Conn
	m       sync.Mutex
}

func newRoom(name string) *room {
	return &room{
		name: name,
	}
}

func (r *room) join(conn net.Conn) {
	r.m.Lock()
	defer r.m.Unlock()

	r.players = append(r.players, conn)
}
