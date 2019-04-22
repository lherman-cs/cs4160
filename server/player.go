package main

import (
	"io"

	"github.com/Pallinder/go-randomdata"
)

type human struct {
	conn    io.ReadWriter
	name    string
	channel chan<- map[string]string
}

func newHuman(conn io.ReadWriter, channel chan<- map[string]string) *human {
	return &human{conn: conn, name: randomdata.SillyName(), channel: channel}
}
