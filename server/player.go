package main

import (
	"io"

	"github.com/Pallinder/go-randomdata"
)

type human struct {
	conn io.ReadWriter
	name string
}

func newHuman(conn io.ReadWriter) *human {
	return &human{conn: conn, name: randomdata.SillyName()}
}
