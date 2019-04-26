package main

import (
	"bytes"
	"testing"
)

func TestEncode(t *testing.T) {
	table := make(map[string]string)
	table["name"] = "Lukas"
	table["age"] = "23"

	var b bytes.Buffer

	err := newEncoder(&b).encode(table)
	if err != nil {
		t.Error(err)
	}

	expected1 := "age:23\tname:Lukas\t\n"
	expected2 := "name:Lukas\tage:23\t\n"
	got := b.String()
	if !(got == expected1 || got == expected2) {
		t.Error("expected1:", expected1, "expected2:", expected2, "got:", got)
	}
}

func TestEncodeReflect(t *testing.T) {
	table := struct {
		Players []string `msg:"name"`
		Ages    []int    `msg:"age"`
	}{Players: []string{"Lukas", "Poop"}, Ages: []int{23, 30}}

	var b bytes.Buffer

	err := newEncoder(&b).encode(table)
	if err != nil {
		t.Error(err)
	}

	expected1 := "age:23,30\tname:Lukas,Poop\t\n"
	expected2 := "name:Lukas,Poop\tage:23,30\t\n"
	got := b.String()
	if !(got == expected1 || got == expected2) {
		t.Error("expected1:", expected1, "expected2:", expected2, "got:", got)
	}
}

func TestDecode(t *testing.T) {
	encoded := "name:Lukas\tage:23\t"
	var b bytes.Buffer
	b.WriteString(encoded)

	decoded := make(map[string]string)
	newDecoder(&b).decode(decoded)
	table := make(map[string]string)
	table["name"] = "Lukas"
	table["age"] = "23"

	for k, v := range table {
		if v != decoded[k] {
			t.Error("expected:", v, "got:", decoded[k])
		}
	}
}
