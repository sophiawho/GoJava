// invalid function call - void return type not assignable
package test

func foo(a int, b float64) {
	println(a,b)
}

func main() {
	var a = foo(5, 4.2)
}