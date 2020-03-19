// valid indexing 
package test

func foo() {
	var a []int
	var b [6]rune
	var c [4]float64
	var d [8]string

	var e int = a[8*2]
	var f rune = b[2/2]
	var g float64 = c[(1*22)+91]
	var h string = d[5]
}
