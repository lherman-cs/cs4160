package main

import (
	"github.com/Pallinder/go-randomdata"
	"github.com/sirupsen/logrus"
	"io"
	"math/rand"
	"net"
	"strconv"
	"time"
)

type bot struct {
	io.Reader
	io.Writer
	*Player
}

func newBot(g *game, id int, name string, dice []int) *bot {
	log := logrus.WithFields(logrus.Fields{
		"player": name,
		"room":   g.name,
		"file":   "bot.go",
	})

	client, server := net.Pipe()
	return &bot{
		Reader: client,
		Writer: server,
		Player: &Player{g, id, name, dice, log},
	}
}

func newRandomBot(g *game) *bot {
	name := randomdata.SillyName()
	return newBot(g, -1, name, make([]int, 5))
}

func newBotFromHuman(h *human) *bot {
	return newBot(h.g, h.id, h.name, h.dice)
}

func (b *bot) Loop() {
	maxDelay := 1 // seconds
	minDelay := 1

	for {
		msg := make(map[string]string)
		if err := newDecoder(b).decode(msg); err != nil {
			break
		}

		t := msg["type"]
		if t == "finish" {
			break
		}

		if t != "state" {
			continue
		}

		turn, _ := strconv.ParseInt(msg["turn"], 10, 64)
		numDice, _ := strconv.ParseInt(msg["num_dices"], 10, 64)
		lastQuantity, _ := strconv.ParseInt(msg["last_quantity"], 10, 64)
		lastFace, _ := strconv.ParseInt(msg["last_face"], 10, 64)

		called := b.maybeCall(numDice, lastQuantity, lastFace)
		if called {
			continue
		}

		if turn != int64(b.id) {
			continue
		}

		delay := rand.Intn(maxDelay) + minDelay
		if delay > maxDelay {
			delay = maxDelay
		}
		time.Sleep(time.Duration(delay) * time.Second)
		b.bet(numDice, lastQuantity, lastFace)
	}

	b.log.Info("exiting...")
}

func (b *bot) maybeCall(numDice, lastQuantity, lastFace int64) bool {
	var match int

	quantity := int(lastQuantity)
	face := int(lastFace)

	for _, die := range b.dice {
		if face == die {
			match++
		}
	}

	// guaranteed it's not a lie
	if quantity <= match {
		return false
	}

	// probability with what we have and absolute 1% chance to always call
	if rand.Intn(quantity) < match || rand.Intn(100) < 1 || quantity == int(numDice) {
		b.g.mailbox <- &eventCall{&event{b}}
		return true
	}
	return false
}

func (b *bot) bet(numDice, lastQuantity, lastFace int64) {
	quantity := int(lastQuantity)
	face := int(lastFace)

	if rand.Intn(100) < 80 || face == 5 {
		quantity++
	} else {
		face++
	}

	b.g.mailbox <- &eventBet{&event{b}, quantity, face}
}
