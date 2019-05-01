package main

import (
	"fmt"
	"io"
	"time"

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
		reason := fmt.Sprint(err)
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

	routerLogger.Debug("received ", msg)
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

	room, err := newGame(mainLobby, name)
	if err != nil {
		panic(err.Error())
	}

	h := newHuman(room, conn)
	room.join(h)
}

// requires:
//	- id: room's id
func join(conn io.ReadWriter, msg map[string]string) {
	id, ok := msg["id"]
	if !ok {
		panic("id is missing")
	}

	room, ok := mainLobby.find(id)
	if !ok {
		panic("room doesn't exist")
	}

	h := newHuman(room, conn)
	err := room.join(h)
	if err != nil {
		panic(err.Error())
	}
}

func subscribe(conn io.ReadWriter, msg map[string]string) {
	source := mainLobby.subscribe(conn)
	lastInfos := mainLobby.getLastInfos()
	timeout := time.Second
	update := func(v interface{}) {
		start := time.Now()
		err := newEncoder(conn).encode(v)
		if err != nil {
			mainLobby.unsubscribe(conn)
			panic(err.Error())
		}
		t := time.Now()
		elapsed := t.Sub(start)
		if elapsed > timeout {
			mainLobby.unsubscribe(conn)
			panic("unsubscribed because timeout")
		}
	}

	if len(lastInfos) > 0 {
		update(mainLobby.getLastInfos())
	}

	for {
		infos := <-source
		update(infos)
	}
}
