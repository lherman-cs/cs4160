package main

import (
	"sync"
	"time"
)

type lobby struct {
	rooms       map[string]*game // <id>:<actual game reference>
	subscribers []chan<- map[string]string
	infos       map[string]string // <id>:<game info in string>
	m           sync.Mutex
}

func newLobby() *lobby {
	return &lobby{rooms: make(map[string]*game), infos: make(map[string]string)}
}

func (l *lobby) subscribe(subscriber chan<- map[string]string) {
	l.m.Lock()
	defer l.m.Unlock()
	l.subscribers = append(l.subscribers, subscriber)
}

func (l *lobby) getLastInfos() map[string]string {
	l.m.Lock()
	defer l.m.Unlock()
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
	l.m.Lock()
	defer l.m.Unlock()
	g, ok = l.rooms[id]
	return
}

func (l *lobby) update(g *game) {
	l.m.Lock()
	defer l.m.Unlock()
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

	filtered := make([]chan<- map[string]string, 0)
	for _, subscriber := range l.subscribers {
		select {
		case subscriber <- copy:
			filtered = append(filtered, subscriber)
		case <-time.After(time.Second * 3):
		}
	}
	l.subscribers = filtered
}
