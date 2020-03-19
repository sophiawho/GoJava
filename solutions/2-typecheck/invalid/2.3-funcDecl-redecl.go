// function declaration - invalid redeclaration
package test

func f1(a int, a float64) {
	println("hello")
}

func f1() {
	println("hi")
}