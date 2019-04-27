package main

type respJoin struct {
	Type  string `msg:"type" default:"join"`
	Index string `msg:"index"`
}
