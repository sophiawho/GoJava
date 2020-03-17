// invalid len builtin: expr not a slice or array
package test

func foo() {
	type point struct {
		a, b int
	}
	var a point
	b := len(a)
}