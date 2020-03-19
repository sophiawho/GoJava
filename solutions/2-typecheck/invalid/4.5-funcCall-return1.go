// invalid function call - return type doesn't match type of assignment variable
package test

func foo(a int, b float64) int {
	return a
}

func main() {
	var a float64 = foo(5, 4.2)
}