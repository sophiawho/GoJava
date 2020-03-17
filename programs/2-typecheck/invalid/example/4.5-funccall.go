// call func foo with too many expressions
package test

func foo (a int) {
	print(true)
}

func main() {
	var b int
	var a int
	foo(a, b)
}