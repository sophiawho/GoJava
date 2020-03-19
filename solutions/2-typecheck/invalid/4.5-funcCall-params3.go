// invalid function call - number of parameters doesn't match function declaration
package test

func foo(a int) int {
	return a
}

func main() {
	foo(5, 2)
}