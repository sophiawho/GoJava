//~1
//~Hello
//~-1.000000e-001

package main

type mystruct struct {
	x1 int
	x2 string
	x3 float64
}

func main() {
	var a mystruct
	a.x1 = 1
	a.x2 = "Hello"
	a.x3 = -0.1
	println(a.x1)
	println(a.x2)
	println(a.x3)
}
