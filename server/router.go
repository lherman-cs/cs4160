package main

import (
	"io"
	"strconv"

	"github.com/google/uuid"
	log "github.com/sirupsen/logrus"
)

func handle(conn io.ReadWriteCloser) {
	defer conn.Close()

	msg := make(map[string]string)
	err := newDecoder(conn).decode(msg)
	if err != nil {
		log.Error("[router] received a malformed message")
		return
	}

	log.Debug("[router] received ", msg)
	cmd, ok := msg["command"]
	if !ok {
		log.Error("[router] command doesn't exist in message")
		return
	}
	log.Debug("[router] received command ", cmd)

	switch cmd {
	case "create-room":
		createRoom(conn, msg)
	case "list-rooms":
		listRooms(conn)
	case "detail-room":
		detailRoom(conn, msg)
	default:
		log.Error("[router] received invalid command")
		return
	}
}

// requires:
//	- name: room's name
func createRoom(conn io.ReadWriter, msg map[string]string) {
	log.Info("[router] handle createRoom command")

	name, ok := msg["name"]
	if !ok {
		log.Error("[router] name is missing")
		return
	}

	id, err := uuid.NewRandom()
	if err != nil {
		log.Error("[router] ", err)
		return
	}

	rooms.Store(id.String(), newRoom(name))
}

// listRooms will reply with the following format
// <id_1>:<room_name_1>\t
// <id_2>:<room_name_2>\t\n
func listRooms(conn io.ReadWriter) {
	log.Info("[router] handle listRooms command")

	msg := make(map[string]string)
	rooms.Range(func(key interface{}, value interface{}) bool {
		k := key.(string)
		v := value.(*room)
		msg[k] = v.name
		return true
	})

	err := newEncoder(conn).encode(msg)
	if err != nil {
		log.Error("[router] failed in encoding message")
		return
	}

	log.Debug("[router] ", msg)
}

// requires:
//	- id: room's id
// response:
//	- name: room's name
//	- num_players: the number of players who have joined
func detailRoom(conn io.ReadWriter, msg map[string]string) {
	log.Info("[router] handle detailRoom command")

	id, ok := msg["id"]
	if !ok {
		log.Error("[router] id is required")
		return
	}

	value, ok := rooms.Load(id)
	if !ok {
		log.Error("[router] room doesn't exist")
		return
	}
	room := value.(*room)
	resp := make(map[string]string)
	resp["name"] = room.name
	resp["num_players"] = strconv.Itoa(len(room.players))

	err := newEncoder(conn).encode(resp)
	if err != nil {
		log.Error("[router] failed in encoding message")
		return
	}

	log.Debug("[router] ", msg)
}
