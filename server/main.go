package main

import (
	"math/rand"
	"net"
	"os"
	"time"

	log "github.com/sirupsen/logrus"
)

const (
	tcpAddr = ":8081"
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

func main() {
	log.SetLevel(log.DebugLevel)
	rand.Seed(time.Now().UTC().UnixNano())
	for {
		startTCPService()
	}
}
