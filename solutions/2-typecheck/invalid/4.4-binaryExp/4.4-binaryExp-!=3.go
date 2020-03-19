// invalid binary exp: != mismatch types array
package test

func foo () {
	var x [5]int
	var y [6]int
	var a bool = x != y;
}