package main

import (
	"net"
	"os"

	log "github.com/sirupsen/logrus"
)

func main() {
	log.SetLevel(log.DebugLevel)
	list, err := net.Listen("tcp", ":8080")
	if err != nil {
		log.Error("[main]", err)
		os.Exit(1)
	}
	defer list.Close()

	for {
		client, err := list.Accept()
		if err != nil {
			log.Warn("[main] ", err)
			continue
		}
		log.Info("[main] received a connection from ", client.RemoteAddr().String())
		go handle(client)
	}
}
