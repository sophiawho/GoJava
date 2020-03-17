// invalid len builtin: expr not string, slice or array
package test

func foo() {
	var a rune
	b := len(a)
}