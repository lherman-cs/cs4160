package main

import (
	"io"
)

// send makes shallow copy of msg and send that
// to w and will send an error (nil if none) to errChan.
// msg CAN'T BE WRITTEN AT THE SAME TIME during the sending
func send(to io.Writer, msg interface{}) <-chan error {
	errChan := make(chan error)
	go func() {
		errChan <- newEncoder(to).encode(msg)
	}()
	return errChan
}
