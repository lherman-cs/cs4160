package main

import (
	"github.com/Pallinder/go-randomdata"
	"github.com/sirupsen/logrus"
	"io"
	"strconv"
)

type human struct {
	io.ReadWriter
	*Player
}

func newHuman(g *game, conn io.ReadWriter) *human {
	name := randomdata.SillyName()
	log := logrus.WithFields(logrus.Fields{
		"player": name,
		"room":   g.name,
		"file":   "human.go",
	})
	h := human{
		ReadWriter: conn,
		Player:     &Player{g, -1, name, make([]int, 5), log},
	}
	return &h
}

func (h *human) Loop() {
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
			send(h, &respError{Reason: reason})
		}
	}()

	cmd, _ := msg["command"]

	var e eventer
	switch cmd {
	case "game-bet":
		quantityStr, ok := msg["quantity"]
		if !ok {
			panic("quantity is required")
		}
		faceStr, ok := msg["face"]
		if !ok {
			panic("face is required")
		}

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
	case "game-call":
		e = &eventCall{&event{from: h}}
	default:
		panic("invalid command")
	}

	h.g.mailbox <- e
}
