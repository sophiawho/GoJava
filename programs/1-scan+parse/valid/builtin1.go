package main

func main() {
	var x [3]int
	x = append(x, 2)
	var y = len(x)
	var z = cap(x)
}
