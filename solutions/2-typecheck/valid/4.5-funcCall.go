// valid function call
package test

func foo1(a int, b float64) int {
	println("hi")
	return a
}

func foo2() {
	println("hey")
}

func main() {
	var a int = foo1(7, 9.2)
	foo2()
}