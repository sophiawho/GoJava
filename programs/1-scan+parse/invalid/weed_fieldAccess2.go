package main

type point struct {
	x int
}

func main() {
	var _ point
	_.x = 1
}