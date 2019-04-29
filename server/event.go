package main

import (
	"time"
)

type eventer interface {
	From() Entity
	Timestamp() time.Time
}

type event struct {
	from      Entity
	timestamp time.Time
}

func newEvent(from Entity) *event {
	return &event{from, time.Now()}
}

func (e *event) From() Entity {
	return e.from
}

func (e *event) Timestamp() time.Time {
	return e.timestamp
}

// command: game-bet
type eventBet struct {
	*event
	quantity int
	face     int
}

// command: game-leave
type eventLeave struct {
	*event
	reason string
}

// command: game-start
type eventStart struct {
	*event
}

// command: game-call
type eventCall struct {
	*event
}

type eventJoin struct {
	*event
	errChan chan<- error
}
