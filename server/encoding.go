package main

import (
	"bufio"
	"fmt"
	"github.com/spf13/cast"
	"io"
	"reflect"
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

func (e *encoder) toString(v interface{}) string {
	value := reflect.ValueOf(v)
	kind := value.Kind()
	switch kind {
	case reflect.Slice:
		slice := make([]string, 0, value.Len())
		for i := 0; i < value.Len(); i++ {
			slice = append(slice, cast.ToString(value.Index(i).Interface()))
		}
		return strings.Join(slice, ",")
	default:
		return cast.ToString(value.Interface())
	}
}

// encode formats a map and returns a string
func (e *encoder) encode(v interface{}) error {
	value := reflect.ValueOf(v)
	kind := value.Kind()

	switch kind {
	case reflect.Map:
		iter := value.MapRange()
		for iter.Next() {
			k := iter.Key().String()
			v := iter.Value().String()
			_, err := e.w.WriteString(k + msgeq + v + msgsep)
			if err != nil {
				return err
			}
		}
	case reflect.Struct:
		st := reflect.TypeOf(v)
		for i := 0; i < st.NumField(); i++ {
			typeField := st.Field(i)
			alias, ok := typeField.Tag.Lookup("msg")
			if !ok {
				continue
			}
			valueStr := e.toString(value.Field(i).Interface())
			e.w.WriteString(alias + msgeq + valueStr + msgsep)
		}
	default:
		return fmt.Errorf("doesn't support %s", kind.String())
	}

	e.w.WriteRune('\n')
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
