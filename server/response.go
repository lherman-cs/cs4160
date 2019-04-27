package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index string `msg:"index"`
}

type respState struct {
	Type     string   `msg:"type" default:"state"`
	Players  []string `msg:"players"`
	Turn     string   `msg:"turn"`
	Round    string   `msg:"round"`
	NumDices string   `msg:"num_dices"`
}
