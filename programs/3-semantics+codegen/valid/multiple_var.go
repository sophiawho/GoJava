//~1 2 0 0
//~0 
//~false
//~15
//~0
//~0 0 0
//~

package test

func main() {
	var a, b = 1, 2
	var c, d int
	println(a, b, c, d)

	var e, f string
	println(len(e), f)

	var g bool
	println(g)

	var h, i, j [5]int
	println(len(h) + len(i) + len(j))

	var k, l []int
	println(len(l))

	type coord struct {
		x, z int
		y int
	}
	var c_1 coord
	println(c_1.x, c_1.y, c_1.z)
}