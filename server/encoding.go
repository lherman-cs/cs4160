package main

import (
	"bufio"
	"fmt"
	"io"
	"strings"
)

const (
	msgsep = "\t"
	msgeq  = ":"
)

type encoder struct {
	w *bufio.Writer
}

func newEncoder(w io.Writer) *encoder {
	return &encoder{w: bufio.NewWriter(w)}
}

// encode formats a map and returns a string
func (e *encoder) encode(table map[string]string) error {
	for k, v := range table {
		_, err := e.w.WriteString(k + msgeq + v + msgsep)
		if err != nil {
			return err
		}
	}
	return e.w.Flush()
}

type decoder struct {
	r *bufio.Scanner
}

func newDecoder(r io.Reader) *decoder {
	return &decoder{r: bufio.NewScanner(r)}
}

// decode parses a string expression and returns a map
func (d *decoder) decode(table map[string]string) error {
	if !d.r.Scan() {
		return fmt.Errorf("invalid message format/stream has ended")
	}

	msg := d.r.Text()
	lines := strings.Split(msg, msgsep)
	for _, line := range lines {
		splits := strings.SplitN(line, msgeq, 2)
		if len(splits) < 2 {
			continue
		}
		k, v := splits[0], splits[1]
		table[k] = v
	}
	return nil
}
