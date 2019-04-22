package main

import (
	"io"
	"strconv"
	"strings"

	"github.com/google/uuid"
	log "github.com/sirupsen/logrus"
)

type handlerFunc func(conn io.ReadWriter, msg map[string]string)

var routerLogger = log.WithFields(log.Fields{
	"file": "router.go",
})

var routes = map[string]handlerFunc{
	"create": create,
	"join":   join,
	"list":   list,
	"detail": detail,
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
	resp := make(map[string]string)
	name, ok := msg["name"]
	if !ok {
		panic("name is missing")
	}

	id, err := uuid.NewRandom()
	if err != nil {
		panic(err.Error())
	}

	room := newGame(name)
	room.join(conn)
	rooms.Store(id.String(), room)
	newEncoder(conn).encode(resp)
}

// requires:
//	- id: room's id
func join(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
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
	newEncoder(conn).encode(resp)
}

// Response:
// 	-	<id_1>:<room_name_1>
// 	-	<id_2>:<room_name_2>
func list(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	rooms.Range(func(key interface{}, value interface{}) bool {
		k := key.(string)
		room := value.(*game)
		resp[k] = room.name
		return true
	})

	newEncoder(conn).encode(resp)
}

// requires:
//	- id: room's id
// response:
//	- name: room's name
//	- num_players: the number of players who have joined
func detail(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	id, ok := msg["id"]
	if !ok {
		panic("id is required")
	}

	value, ok := rooms.Load(id)
	if !ok {
		panic("room doesn't exist")
	}
	room := value.(*game)
	players := room.joinedPlayers()
	resp["name"] = room.name
	resp["players"] = strings.Join(players, ",")
	resp["num_players"] = strconv.Itoa(len(players))
	newEncoder(conn).encode(resp)
}
