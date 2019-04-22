package main

import (
	"io"
)

type human struct {
	conn io.ReadWriter
	name string
}

func newHuman(conn io.ReadWriter, name string) *human {
	return &human{conn: conn, name: name}
}
