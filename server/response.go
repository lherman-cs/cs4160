package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index int    `msg:"index"`
}

type respState struct {
	Type         string   `msg:"type" default:"state"`
	Started      bool     `msg:"started"` // true or false
	Players      []string `msg:"players"`
	Turn         int      `msg:"turn"`
	Round        int      `msg:"round"`
	NumDices     int      `msg:"num_dices"`
	LastQuantity int      `msg:"last_quantity"`
	LastFace     int      `msg:"last_face"`
}

type respStart struct {
	Type string `msg:"type" default:"start"`
}

type respError struct {
	Type   string `msg:"type" default:"error"`
	Reason string `msg:"reason"`
}
