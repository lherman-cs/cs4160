package main

import (
	"bytes"
	"io"
	"net"
	"sync/atomic"
	"testing"
	"time"
)

func BenchmarkThousandTCP(b *testing.B) {
	n := 1000
	endpoint := ":8081"
	var ops uint64
	done := make(chan struct{})

	go func() {

	}()

	for i := 0; i < n; i++ {
		go func() {
			conn, err := net.Dial("tcp", endpoint)
			if err != nil {
				b.Fatal(err)
			}
			defer conn.Close()

			var buff bytes.Buffer
			buff.WriteString("command:subscribe\t\n")
			_, err = io.Copy(conn, &buff)
			if err != nil {
				b.Fatal(err)
			}
			atomic.AddUint64(&ops, 1)
			<-done
		}()
		time.Sleep(time.Millisecond)
	}

	for {
		time.Sleep(time.Second * 3)
		b.Log(atomic.LoadUint64(&ops), " subscribers")
	}
}
