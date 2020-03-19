// invalid function call - parameter types don't match function declaration
package test

func foo(a int, b float64) int {
	return a
}

func main() {
	foo(5.1, 4.2)
}