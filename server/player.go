package main

import (
	"io"
)

// Entity an abstraction that represents entity in the game
type Entity interface {
	io.ReadWriter
	Loop()
	Name() string
	Dice() []int
}

// Player is a concrete base type that should be composed of.
// Player implements sofe of Entity methods
type Player struct {
	g    *game
	name string
	dice []int
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
