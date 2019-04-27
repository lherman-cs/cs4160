package main

import (
	"strconv"
)

type status uint8

type bet struct {
	quantity int
	face     int
}

type state struct {
	started    bool
	finished   bool
	lastBet    bet
	players    []*human
	turn       int
	round      int
	numDices   int
	calledLiar bool // to protect multiple people call liar at the same turn
}

func (s *state) encode() *respState {
	players := make([]string, 0, len(s.players))
	for _, p := range s.players {
		players = append(players, p.name)
	}

	return &respState{
		Players:  players,
		Started:  strconv.FormatBool(s.started),
		Turn:     strconv.Itoa(s.turn),
		Round:    strconv.Itoa(s.round),
		NumDices: strconv.Itoa(s.numDices),
	}
}
