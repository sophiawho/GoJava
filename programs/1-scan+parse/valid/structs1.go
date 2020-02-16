package main
type point1 struct {
	x, y, z int
}

func main() {
	var p1 point1
	p1.x = 1 
	p1.y = 1
	p1.z = 1 

	type point2 struct {
		x, y int
	}

	var p2 point2
	p2.x = 1
	p2.y = 1
}
