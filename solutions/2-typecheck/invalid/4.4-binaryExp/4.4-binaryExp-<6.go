// invalid binary exp: < mismatch types int and float
package test

func foo () {
	var x int = 5
	var y float64 = 5.
	var a bool = x < y;
}