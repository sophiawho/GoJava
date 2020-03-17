// assignment - field of struct not selected
package test

func foo() {
	type point struct { x,y,z int; }
	var p point
	p = 12
}
