package main

type point struct {
	_ int
}

func main() {
	var a point
	a._ = 1
}