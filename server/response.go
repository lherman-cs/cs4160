package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index int    `msg:"index"`
}

type respState struct {
	Type         string   `msg:"type" default:"state"`
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

type respRoll struct {
	Type  string   `msg:"type" default:"roll"`
	Zero  []string `msg:"0"`
	One   []string `msg:"1"`
	Two   []string `msg:"2"`
	Three []string `msg:"3"`
	Four  []string `msg:"4"`
}

type respFinish struct {
	Type string `msg:"type" default:"finish"`
}

type respCall struct {
	Type string `msg:"type" default:"call"`
}

type respError struct {
	Type   string `msg:"type" default:"error"`
	Reason string `msg:"reason"`
}
