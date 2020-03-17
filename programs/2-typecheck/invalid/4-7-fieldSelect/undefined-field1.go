// Select undefined field
package test

type struct_type struct {
	x int
	y int
}

func main() {
	var a struct_type
	println(a.z)
}