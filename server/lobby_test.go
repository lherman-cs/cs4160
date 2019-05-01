package main

import (
	"io"
	"net"
	"os"
	"sync"
	"testing"
	"time"
)

func BenchmarkThousandTCP(b *testing.B) {
	n := 10000
	endpoint := ":8081"
	done := make(chan struct{})
	var wg sync.WaitGroup

	create := func(name string) {
		conn, err := net.Dial("tcp", endpoint)
		if err != nil {
			b.Fatal(err)
		}
		defer conn.Close()

		req := map[string]string{
			"command": "create",
			"name":    name,
		}

		if err = newEncoder(conn).encode(req); err != nil {
			b.Fatal(err)
		}
		close(done)
	}

	subscribe := func(wg *sync.WaitGroup) {
		conn, err := net.Dial("tcp", endpoint)
		if err != nil {
			b.Fatal(err)
		}
		defer conn.Close()

		req := map[string]string{"command": "subscribe"}
		err = newEncoder(conn).encode(req)
		if err != nil {
			b.Fatal(err)
		}
		wg.Done()

		_, err = io.Copy(os.Stdout, conn)
		if err != nil {
			b.Fatal(err)
		}
		<-done
	}

	go startTCPService()

	time.Sleep(time.Second)
	wg.Add(n)
	for i := 0; i < n; i++ {
		go subscribe(&wg)
		time.Sleep(time.Millisecond)
	}
	wg.Wait()
	create("Lukas's Room")
}
