package main

import (
	"io"

	"github.com/Pallinder/go-randomdata"
	"github.com/sirupsen/logrus"
)

type human struct {
	g     *game
	conn  io.ReadWriter
	name  string
	owner bool
	log   *logrus.Entry
}

func newHuman(g *game, conn io.ReadWriter, owner bool) *human {
	name := randomdata.SillyName()
	log := logrus.WithFields(logrus.Fields{
		"player": name,
		"room":   g.name,
		"owner":  owner,
	})
	h := human{g: g, conn: conn, name: name, owner: owner, log: log}
	return &h
}

func (h *human) loop() {
	receiver := make(chan map[string]string)

	go func() {
		var err error
		for {
			msg := make(map[string]string)
			err = newDecoder(h.conn).decode(msg)
			if err != nil {
				h.log.Error(err)
				break
			}

			h.log.Debug("received: ", msg)
			receiver <- msg
		}
	}()

loop:
	for {
		select {
		case msg := <-receiver:
			h.handle(msg)
		case <-h.g.done:
			break loop
		}
	}
}

func (h *human) handle(msg map[string]string) {
	// embed player's name
	msg["player"] = h.name
	// TODO! do verification first before sending it to the game
	h.g.mailbox <- msg
}
