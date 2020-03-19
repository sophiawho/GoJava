// invalid len: first expression not a slice, array, or string
package test

func foo() {
	type point struct {
		x,y int
	}
	var a point
	b := len(a)
}