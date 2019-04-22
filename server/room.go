package main

import "net"

type room struct {
	name    string
	players []net.Conn
}

func newRoom(name string) *room {
	return &room{
		name: name,
	}
}
