// assignment - invalid lvalue: non-addressable LHS of assign
package test

func foo() int {
	return 4
}

func foo2() {
	foo() = 3
}