package main

import (
	"net"

	"github.com/google/uuid"

	log "github.com/sirupsen/logrus"
)

func handle(conn net.Conn) {
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

	switch cmd {
	case "create-room":
		createRoom(conn, msg)
	case "list-rooms":
		listRooms(conn)
	default:
		log.Error("[router] received invalid command")
		return
	}
}

func createRoom(conn net.Conn, msg map[string]string) {
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
func listRooms(conn net.Conn) {
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
