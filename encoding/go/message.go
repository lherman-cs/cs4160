package message

import (
	"strings"
)

const (
	msgsep = "\r"
	msgeq  = ":"
)

// Encode formats a map and returns a string
func Encode(table map[string]string) string {
	var b strings.Builder

	for k, v := range table {
		b.WriteString(k + msgeq + v + msgsep)
	}
	return b.String()
}

// Decode parses a string expression and returns a map
func Decode(msg string) map[string]string {
	table := make(map[string]string)
	lines := strings.Split(msg, msgsep)
	for _, line := range lines {
		splits := strings.SplitN(line, msgeq, 2)
		if len(splits) < 2 {
			continue
		}
		k, v := splits[0], splits[1]
		table[k] = v
	}
	return table
}
