// invalid binary exp: > unorderable type arrays
package test

func foo () {
	var x [5]int
	var y [5]int
	var a bool = x > y;
}