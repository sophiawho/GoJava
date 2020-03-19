// invalid binary exp: == mismatch types slices
package test

func foo () {
	var x []int
	var y []float64
	var a bool = x == y;
}