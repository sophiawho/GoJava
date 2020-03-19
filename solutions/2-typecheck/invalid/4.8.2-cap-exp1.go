// invalid cap: first expression not a slice or array
package test

func foo() {
	type point struct {
		x,y int
	}
	var a point
	b := cap(a)
}