package main

type eventer interface {
	From() *human
}

type event struct {
	from *human
}

func (e *event) From() *human {
	return e.from
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
