//~8 9 10
//~0
//~5
//~

package main

func init() {
	var a, b, c int
	a = 10
	b = 9
	c = 8
	a, b, c = c, b, a
	println(a, b, c)
}

func main() {
	var a, b int
	a = 5
	_ = 5
	a, b = b, a
	println(a)
	println(b)	
}