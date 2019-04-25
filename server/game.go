package main

import (
	"fmt"
	"io"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/sirupsen/logrus"
)

const (
	maxPlayers = 5
)

type game struct {
	state
	name    string
	mailbox chan<- map[string]string
	done    <-chan struct{}
	log     *logrus.Entry
	m       sync.Mutex
}

func newGame(name string) *game {
	mailbox := make(chan map[string]string, 64)
	done := make(chan struct{})
	log := logrus.WithFields(logrus.Fields{
		"room": name,
	})
	game := game{
		name:    name,
		mailbox: mailbox,
		done:    done,
		log:     log,
	}
	go game.loop(mailbox, done)

	return &game
}

func (g *game) join(conn io.ReadWriter) error {
	g.m.Lock()
	if len(g.players) == maxPlayers {
		g.m.Unlock()
		return fmt.Errorf("game is already full")
	}

	h := newHuman(g, conn)
	g.players = append(g.players, h)
	g.log.Info(h.name, " joined")
	newEncoder(conn).encode(map[string]string{})
	g.m.Unlock()
	mainLobby.notifyAll()
	h.loop()
	return nil
}

// info gives the game's detail in the following format:
// <name>,<num_players>
func (g *game) info() string {
	return g.name + "," + strconv.Itoa(len(g.players))
}

// joinedPlayers get names who have joined
func (g *game) joinedPlayers() []string {
	g.m.Lock()
	defer g.m.Unlock()

	players := make([]string, 0, len(g.players))
	for _, p := range g.players {
		players = append(players, p.name)
	}

	return players
}

func (g *game) loop(mailbox <-chan map[string]string, done chan<- struct{}) {
	defer close(done)

	broadcast := func() {
		currentState := g.encode()

		dones := make([]chan struct{}, len(g.players))
		for i, p := range g.players {
			dones[i] = make(chan struct{})
			go func(done chan<- struct{}, p *human) {
				newEncoder(p.conn).encode(currentState)
				close(done)
			}(dones[i], p)
		}

		for i := range g.players {
			select {
			case <-dones[i]:
			case <-time.After(time.Second * 10):
				// TODO! Remove player from the game
			}
		}
	}

	for {
		msg := <-mailbox // TODO! handle msg
		changed := g.handle(msg)

		if changed {
			broadcast() // update players with current state
		}
	}
}

func (g *game) handle(msg map[string]string) (changed bool) {
	before := g.state
	defer func() {
		if err := recover(); err != nil {
			reason := err.(string)
			g.log.WithField("player", msg["player"]).Error(reason)
			// rollback state
			g.state = before
			changed = false
		}
	}()

	cmd, ok := msg["command"]
	if !ok {
		panic("command is required")
	}

	switch cmd {
	case "game-bet":
		g.bet(msg)
	default:
		panic("invalid command")
	}

	return true
}

// requires:
//	- command:game-bet
//	- quantity:<number>
//	- face:<number>
func (g *game) bet(msg map[string]string) {
	quantityStr := msg["quantity"]
	faceStr := msg["face"]

	quantity, err := strconv.Atoi(quantityStr)
	if err != nil {
		panic("quantity is not an integer")
	}

	face, err := strconv.Atoi(faceStr)
	if err != nil {
		panic("face is not an integer")
	}

	g.lastBet.quantity = quantity
	g.lastBet.face = face
}

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
