package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index int    `msg:"index"`
}

type respState struct {
	Type     string   `msg:"type" default:"state"`
	Started  bool     `msg:"started"` // true or false
	Players  []string `msg:"players"`
	Turn     int      `msg:"turn"`
	Round    int      `msg:"round"`
	NumDices int      `msg:"num_dices"`
}

type respStart struct {
	Type string `msg:"type" default:"start"`
}
