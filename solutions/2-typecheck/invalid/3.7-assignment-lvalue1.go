// assignment - invalid lvalue: non-addressable LHS of assign
package test

func foo() {
	var a int
	a+3 = 8
}