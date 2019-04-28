package main

import (
	"bytes"
	"github.com/Pallinder/go-randomdata"
	"github.com/sirupsen/logrus"
	"io"
)

type bot struct {
	io.ReadWriter
	*Player
}

func newBot(g *game, id int, name string, dice []int) *bot {
	var buff bytes.Buffer
	log := logrus.WithFields(logrus.Fields{
		"player": name,
		"room":   g.name,
		"file":   "bot.go",
	})

	return &bot{
		ReadWriter: &buff,
		Player:     &Player{g, id, name, dice, log},
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

}
