package main

import (
	"sync"
	"time"
)

type lobby struct {
	subscribers []chan<- map[string]string
	m           sync.Mutex
}

func (l *lobby) subscribe(subscriber chan<- map[string]string) {
	l.m.Lock()
	defer l.m.Unlock()

	l.subscribers = append(l.subscribers, subscriber)
}

func (l *lobby) notifyAll() {
	l.m.Lock()
	defer l.m.Unlock()

	resp := make(map[string]string)
	rooms.Range(func(key interface{}, value interface{}) bool {
		k := key.(string)
		room := value.(*game)
		resp[k] = room.info()
		return true
	})

	filtered := make([]chan<- map[string]string, 0)
	for _, subscriber := range l.subscribers {
		select {
		case subscriber <- resp:
			filtered = append(filtered, subscriber)
		case <-time.After(time.Second * 3):
		}
	}
	l.subscribers = filtered
}
