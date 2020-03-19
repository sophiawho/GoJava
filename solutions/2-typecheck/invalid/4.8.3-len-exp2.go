// invalid len: first expression not a slice, array, or string
package test

func foo() {
	var a int
	b := len(a)
}