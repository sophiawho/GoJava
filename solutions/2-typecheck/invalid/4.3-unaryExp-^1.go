// invalid unary expression: ^ invalid float arg
package test

func foo() {
	var a float64
	println(^a)
}