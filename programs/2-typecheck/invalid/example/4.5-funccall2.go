// call func foo with too few expressions: expects 1, calls with 0
package test

func foo (a int) {
	print(true)
}

func main() {
	var b int
	var a int
	foo()
}