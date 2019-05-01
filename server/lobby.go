package main

import (
	"io"
	"sync"

	log "github.com/sirupsen/logrus"
)

type lobby struct {
	rooms       map[string]*game // <id>:<actual game reference>
	subscribers map[io.Writer]chan<- map[string]string
	infos       map[string]string // <id>:<game info in string>
	m           sync.RWMutex
}

func newLobby() *lobby {
	return &lobby{
		rooms:       make(map[string]*game),
		subscribers: make(map[io.Writer]chan<- map[string]string),
		infos:       make(map[string]string)}
}

func (l *lobby) subscribe(subscriber io.Writer) <-chan map[string]string {
	l.m.Lock()
	defer l.m.Unlock()
	// make sure that it's not blocking
	c := make(chan map[string]string, 16)
	l.subscribers[subscriber] = c
	return c
}

func (l *lobby) unsubscribe(subscriber io.Writer) {
	l.m.Lock()
	defer l.m.Unlock()
	delete(l.subscribers, subscriber)
}

func (l *lobby) getLastInfos() map[string]string {
	l.m.RLock()
	defer l.m.RUnlock()
	copy := make(map[string]string)
	for k, v := range l.infos {
		copy[k] = v
	}

	return copy
}

func (l *lobby) add(newRoom *game) {
	l.m.Lock()
	defer l.m.Unlock()
	l.rooms[newRoom.id] = newRoom
	l.infos[newRoom.id] = newRoom.info()
	l.notifyAll()
}

func (l *lobby) find(id string) (g *game, ok bool) {
	l.m.RLock()
	defer l.m.RUnlock()
	g, ok = l.rooms[id]
	return
}

func (l *lobby) update(g *game) {
	l.m.Lock()
	defer l.m.Unlock()
	if g.started {
		return
	}
	l.infos[g.id] = g.info()
	l.notifyAll()
}

func (l *lobby) close(g *game) {
	l.m.Lock()
	defer l.m.Unlock()
	delete(l.rooms, g.id)
	delete(l.infos, g.id)
	l.notifyAll()
}

// notifyAll should be only used within lobby
func (l *lobby) notifyAll() {
	copy := make(map[string]string)
	for k, v := range l.infos {
		copy[k] = v
	}

	log.Info("broadcasting to ", len(l.subscribers), " subscribers")
	for _, subscriber := range l.subscribers {
		subscriber <- copy
	}
}
