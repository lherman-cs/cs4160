package main

import (
	"strconv"
	"strings"
)

type status uint8

type bet struct {
	quantity int
	face     int
}

type state struct {
	lastBet    bet
	players    []*human
	turn       int
	round      int
	numDices   int
	calledLiar bool // to protect multiple people call liar at the same turn
}

func (s *state) encode() map[string]string {
	players := make([]string, 0, len(s.players))
	for _, p := range s.players {
		players = append(players, p.name)
	}

	return map[string]string{
		"players":   strings.Join(players, ","),
		"turn":      strconv.Itoa(s.turn),
		"round":     strconv.Itoa(s.round),
		"num_dices": strconv.Itoa(s.numDices),
	}
}
