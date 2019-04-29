package main

import (
	"io"

	"github.com/sirupsen/logrus"
)

// Entity an abstraction that represents entity in the game
type Entity interface {
	io.ReadWriter
	Loop()
	Name() string
	Dice() []int
	// Remove 1 die from the entity
	LoseDie()
	GetID() int
	SetID(id int)
}

// Player is a concrete base type that should be composed of.
// Player implements sofe of Entity methods
type Player struct {
	g    *game
	id   int
	name string
	dice []int
	log  *logrus.Entry
}

// GetID gets player's id
func (p *Player) GetID() int {
	return p.id
}

// SetID sets player's id
func (p *Player) SetID(id int) {
	p.id = id
}

// Name is a getter
func (p *Player) Name() string {
	return p.name
}

// Dice gives a reference to the actual dice
// so that it can be read and written
func (p *Player) Dice() []int {
	return p.dice
}

// LoseDie removes a die from dice
func (p *Player) LoseDie() {
	p.dice = p.dice[:len(p.dice)-1]
}
