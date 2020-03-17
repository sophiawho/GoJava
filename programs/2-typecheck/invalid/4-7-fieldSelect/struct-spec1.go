// Two fields have same identifiers
package test

type struct_type struct {
	x, y int
	y int
}

func main() {
	var a struct_type
	println(a.y)
}