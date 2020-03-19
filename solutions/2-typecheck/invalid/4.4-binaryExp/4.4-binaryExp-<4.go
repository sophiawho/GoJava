// invalid binary exp: < unorderable type slices
package test

func foo () {
	var x []int
	var y []int
	var a bool = x < y;
}