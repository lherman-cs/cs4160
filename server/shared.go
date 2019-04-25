package main

import "sync"

var rooms sync.Map // room_id:*game
var mainLobby lobby
