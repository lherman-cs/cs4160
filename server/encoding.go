package main

import (
	"bufio"
	"fmt"
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

			valueField := value.Field(i)
			kind = valueField.Kind()
			valueStr := ""
			switch kind {
			case reflect.String:
				valueStr = valueField.String()
			case reflect.Slice:
				slice, ok := valueField.Interface().([]string)
				if !ok {
					return fmt.Errorf("doesn't support slice other than []string")
				}
				valueStr = strings.Join(slice, ",")
			}
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
func (d *decoder) decode(v interface{}) error {
	if !d.r.Scan() {
		return fmt.Errorf("invalid message format/stream has ended")
	}

	handle := func(msg string, table map[string]string) {
		lines := strings.Split(msg, msgsep)
		for _, line := range lines {
			splits := strings.SplitN(line, msgeq, 2)
			if len(splits) < 2 {
				continue
			}
			k, v := splits[0], splits[1]
			table[k] = v
		}
	}

	msg := d.r.Text()
	value := reflect.ValueOf(v)
	kind := value.Kind()

	table, ok := v.(map[string]string)
	if ok {
		handle(msg, table)
		return nil
	}

	table = make(map[string]string)
	handle(msg, table)
	value = value.Elem()
	kind = value.Kind()
	if kind != reflect.Struct {
		return fmt.Errorf("can't support %s", kind.String())
	}

	st := value.Type()
	for i := 0; i < st.NumField(); i++ {
		typeField := st.Field(i)
		alias, ok := typeField.Tag.Lookup("msg")
		if !ok {
			continue
		}

		rawValue, ok := table[alias]
		if !ok {
			return fmt.Errorf("%s doesn't exist", alias)
		}

		valueField := value.Field(i)
		valueKind := valueField.Kind()
		switch valueKind {
		case reflect.String:
			valueField.SetString(rawValue)
		default:
			return fmt.Errorf("decode doesn't support non string struct fields")
		}
	}

	return nil
}
