// assignment - invalid lvalue: non-addressable LHS of assign
package test

func foo() {
	var a string
	a+"hi" = "hello"
}