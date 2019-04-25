package main

import (
	"io"

	"github.com/google/uuid"
	log "github.com/sirupsen/logrus"
)

type handlerFunc func(conn io.ReadWriter, msg map[string]string)

var routerLogger = log.WithFields(log.Fields{
	"file": "router.go",
})

var routes = map[string]handlerFunc{
	"create":    create,
	"join":      join,
	"subscribe": subscribe,
}

func routerRecover(conn io.ReadWriter) {
	if err := recover(); err != nil {
		reason := err.(string)
		resp := map[string]string{"error": reason}
		routerLogger.Error(reason)
		newEncoder(conn).encode(resp)
	}
}

func handle(conn io.ReadWriteCloser) {
	defer conn.Close()
	defer routerRecover(conn)

	msg := make(map[string]string)
	err := newDecoder(conn).decode(msg)
	if err != nil {
		panic("received a malformed message")
	}

	cmd, ok := msg["command"]
	if !ok {
		panic("command doesn't exist in message")
	}

	handle, ok := routes[cmd]
	if !ok {
		panic("received invalid command")
	}
	handle(conn, msg)
}

// requires:
//	- name: room's name
func create(conn io.ReadWriter, msg map[string]string) {
	name, ok := msg["name"]
	if !ok {
		panic("name is missing")
	}

	id, err := uuid.NewRandom()
	if err != nil {
		panic(err.Error())
	}

	room := newGame(name)
	rooms.Store(id.String(), room)
	mainLobby.notifyAll()
	room.join(conn)
}

// requires:
//	- id: room's id
func join(conn io.ReadWriter, msg map[string]string) {
	id, ok := msg["id"]
	if !ok {
		panic("id is missing")
	}

	value, ok := rooms.Load(id)
	if !ok {
		panic("room doesn't exist")
	}

	room := value.(*game)
	err := room.join(conn)
	if err != nil {
		panic(err.Error())
	}
}

func subscribe(conn io.ReadWriter, msg map[string]string) {
	respChan := make(chan map[string]string)
	mainLobby.subscribe(respChan)

	for {
		resp := <-respChan
		err := newEncoder(conn).encode(resp)
		if err != nil {
			panic(err.Error())
		}
	}
}
