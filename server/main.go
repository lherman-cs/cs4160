package main

import (
	"net"
	"net/http"
	"os"

	"golang.org/x/net/websocket"

	log "github.com/sirupsen/logrus"
)

const (
	tcpAddr = ":8081"
	wsAddr  = ":8080"
)

func startTCPService() {
	defer func() {
		if r := recover(); r != nil {
			log.Error("[tcp] service exited with panic, restarting...")
		}
	}()

	list, err := net.Listen("tcp", tcpAddr)
	if err != nil {
		log.Error("[tcp]", err)
		os.Exit(1)
	}
	defer list.Close()

	for {
		client, err := list.Accept()
		if err != nil {
			log.Warn("[tcp] ", err)
			continue
		}
		log.Info("[tcp] received a connection from ", client.RemoteAddr().String())
		go handle(client)
	}
}

func startWSService() {
	defer func() {
		if r := recover(); r != nil {
			log.Error("[ws] service exited with panic, restarting...")
		}
	}()

	http.Handle("/", websocket.Handler(func(ws *websocket.Conn) {
		log.Info("[ws] received a connection from ", ws.RemoteAddr().String())
		go handle(ws)
	}))
	err := http.ListenAndServe(wsAddr, nil)
	if err != nil {
		log.Error("[ws] websocket service failed")
	}
}

func main() {
	log.SetLevel(log.DebugLevel)
	go func() {
		for {
			startTCPService()
		}
	}()

	for {
		startWSService()
	}
}
