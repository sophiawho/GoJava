// assignment - invalid lvalue: non-addressable LHS of assign
package test

func foo() {
	var b bool
	!b = true
}