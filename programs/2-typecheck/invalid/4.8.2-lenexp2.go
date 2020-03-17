// invalid len builtin: expr not slice, array, or string
package test

func foo() {
	var a int
	b := len(a)
}