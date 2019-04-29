package main

import (
	"time"
)

type bet struct {
	quantity int
	face     int
}

type state struct {
	lastTimestamp time.Time
	started       bool
	finished      bool
	lastBet       bet
	lastPlayer    Entity
	players       []Entity
	turn          int
	round         int
	numDices      int
	calledLiar    bool // to protect multiple people call liar at the same turn
}

func (s *state) encode() *respState {
	players := make([]string, 0, len(s.players))
	for _, p := range s.players {
		players = append(players, p.Name())
	}

	return &respState{
		Players:      players,
		Turn:         s.turn,
		Round:        s.round,
		NumDices:     s.numDices,
		LastQuantity: s.lastBet.quantity,
		LastFace:     s.lastBet.face,
	}
}

func (s *state) incrementTurn() {
	for {
		s.turn = (s.turn + 1) % len(s.players)
		if len(s.players[s.turn].Dice()) != 0 {
			break
		}
	}
}
