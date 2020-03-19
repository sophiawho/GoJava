// print - not base type: struct
package test

func foo() {
	type point struct { x, y int; }
	var a point
	print(a)
}
