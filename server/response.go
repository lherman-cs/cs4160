package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index string `msg:"index"`
}

type respState struct {
	Type     string   `msg:"type" default:"state"`
	Started  string   `msg:"started"` // true or false
	Players  []string `msg:"players"`
	Turn     string   `msg:"turn"`
	Round    string   `msg:"round"`
	NumDices string   `msg:"num_dices"`
}

type respStart struct {
	Type string `msg:"type" default:"start"`
}
