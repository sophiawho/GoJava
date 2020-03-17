// invalid cap builtin: expr not slice or array
package test

func foo() {
	var a string
	b := cap(a)
}