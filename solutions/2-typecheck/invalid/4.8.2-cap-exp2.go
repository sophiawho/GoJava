// invalid cap: first expression not a slice or array
package test

func foo() {
	var a int
	b := cap(a)
}