package main

import (
	"io"
	"strconv"
	"strings"

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
// response:
//	- error: ok | <reason>
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

	room := newRoom(name)
	room.join(conn)
	rooms.Store(id.String(), room)
	newEncoder(conn).encode(map[string]string{
		"error": "ok",
	})
}

// listRooms will reply with the following format
// <id_1>:<room_name_1>\t
// <id_2>:<room_name_2>\t\n
// error: ok | <reason>
func listRooms(conn io.ReadWriter) {
	log.Info("[router] handle listRooms command")

	resp := make(map[string]string)
	defer newEncoder(conn).encode(resp)
	resp["error"] = "ok"
	rooms.Range(func(key interface{}, value interface{}) bool {
		k := key.(string)
		v := value.(*room)
		resp[k] = v.name
		return true
	})

	log.Debug("[router] ", resp)
}

// requires:
//	- id: room's id
// response:
//	- name: room's name
//	- num_players: the number of players who have joined
//	- error: ok | <reason>
func detailRoom(conn io.ReadWriter, msg map[string]string) {
	log.Info("[router] handle detailRoom command")

	resp := make(map[string]string)
	defer newEncoder(conn).encode(resp)
	id, ok := msg["id"]
	if !ok {
		err := "id is required"
		resp["error"] = err
		log.Error("[router] ", err)
		return
	}

	value, ok := rooms.Load(id)
	if !ok {
		err := "room doesn't exist"
		resp["error"] = err
		log.Error("[router] ", err)
		return
	}
	room := value.(*room)
	players := room.joinedPlayers()
	resp["name"] = room.name
	resp["players"] = strings.Join(players, ",")
	resp["num_players"] = strconv.Itoa(len(players))
	resp["error"] = "ok"
	log.Debug("[router] ", msg)
}
