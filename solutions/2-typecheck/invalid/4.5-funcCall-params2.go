// invalid function call - number of parameters doesn't match function declaration
package test

func foo(a int, b float64) int {
	return a
}

func main() {
	foo(5)
}