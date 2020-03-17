// Two fields have same identifiers
package test

type struct_type struct {
	x int
	x int
}

func main() {
	var a struct_type
	println(a.x)
}