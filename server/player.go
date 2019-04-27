package main

import (
	"io"

	"github.com/Pallinder/go-randomdata"
	"github.com/sirupsen/logrus"
	"strconv"
)

type human struct {
	io.ReadWriter
	g    *game
	name string
	log  *logrus.Entry
}

func newHuman(g *game, conn io.ReadWriter) *human {
	name := randomdata.SillyName()
	log := logrus.WithFields(logrus.Fields{
		"player": name,
		"room":   g.name,
		"file":   "player.go",
	})
	h := human{g: g, ReadWriter: conn, name: name, log: log}
	return &h
}

func (h *human) loop() {
	receiver := make(chan map[string]string)

	go func() {
		// if there's error, send last message to handler
		defer func() {
			if err := recover(); err != nil {
				reason := err.(string)
				resp := make(map[string]string)
				resp["error"] = reason
				receiver <- resp
			}
		}()

		var err error
		for {
			msg := make(map[string]string)
			err = newDecoder(h).decode(msg)
			if err != nil {
				panic(err.Error())
			}

			receiver <- msg
		}
	}()

loop:
	for {
		select {
		case msg := <-receiver:
			err, ok := msg["error"]
			if ok {
				h.g.mailbox <- &eventLeave{&event{from: h}, err}
				break loop
			}
			h.handle(msg)
		case <-h.g.done:
			break loop
		}
	}
}

func (h *human) handle(msg map[string]string) {
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			resp := map[string]string{"error": reason}
			newEncoder(h).encode(resp)
		}
	}()

	cmd, _ := msg["command"]

	var e eventer
	switch cmd {
	case "game-bet":
		quantityStr := msg["quantity"]
		faceStr := msg["face"]

		quantity, err := strconv.Atoi(quantityStr)
		if err != nil {
			panic("quantity is not an integer")
		}

		face, err := strconv.Atoi(faceStr)
		if err != nil {
			panic("face is not an integer")
		}

		e = &eventBet{&event{from: h}, quantity, face}
	case "game-leave":
		reason := msg["reason"]
		e = &eventLeave{&event{from: h}, reason}
	case "game-start":
		e = &eventStart{&event{from: h}}
	default:
		panic("invalid command")
	}

	h.g.mailbox <- e
}
