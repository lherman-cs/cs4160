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
	"create-room": createRoom,
	"join-room":   joinRoom,
	"list-rooms":  listRooms,
	"detail-room": detailRoom,
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
func createRoom(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	name, ok := msg["name"]
	if !ok {
		panic("name is missing")
	}

	id, err := uuid.NewRandom()
	if err != nil {
		panic(err.Error())
	}

	room := newRoom(name)
	room.join(conn)
	rooms.Store(id.String(), room)
	newEncoder(conn).encode(resp)
}

// requires:
//	- id: room's id
func joinRoom(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	id, ok := msg["id"]
	if !ok {
		panic("id is missing")
	}

	value, ok := rooms.Load(id)
	if !ok {
		panic("room doesn't exist")
	}

	room := value.(*room)
	err := room.join(conn)
	if err != nil {
		panic(err.Error())
	}
	newEncoder(conn).encode(resp)
}

// Response:
// 	-	<id_1>:<room_name_1>
// 	-	<id_2>:<room_name_2>
func listRooms(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	rooms.Range(func(key interface{}, value interface{}) bool {
		k := key.(string)
		v := value.(*room)
		resp[k] = v.name
		return true
	})

	newEncoder(conn).encode(resp)
}

// requires:
//	- id: room's id
// response:
//	- name: room's name
//	- num_players: the number of players who have joined
func detailRoom(conn io.ReadWriter, msg map[string]string) {
	resp := make(map[string]string)
	defer newEncoder(conn).encode(resp)
	id, ok := msg["id"]
	if !ok {
		panic("id is required")
	}

	value, ok := rooms.Load(id)
	if !ok {
		panic("room doesn't exist")
	}
	room := value.(*room)
	players := room.joinedPlayers()
	resp["name"] = room.name
	resp["players"] = strings.Join(players, ",")
	resp["num_players"] = strconv.Itoa(len(players))
	newEncoder(conn).encode(resp)
}
