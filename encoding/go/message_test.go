package message

import (
	"testing"
)

func TestEncode(t *testing.T) {
	table := make(map[string]string)
	table["name"] = "Lukas"
	table["age"] = "23"

	encoded := Encode(table)
	expected := "name:Lukas\rage:23\r"
	if encoded != expected {
		t.Error("expected:", expected, "got:", encoded)
	}
}

func TestDecode(t *testing.T) {
	encoded := "name:Lukas\rage:23\r"
	decoded := Decode(encoded)
	table := make(map[string]string)
	table["name"] = "Lukas"
	table["age"] = "23"

	for k, v := range table {
		if v != decoded[k] {
			t.Error("expected:", v, "got:", decoded[k])
		}
	}
}
