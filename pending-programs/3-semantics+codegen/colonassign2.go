//~5
//~0
//~0
//~5
//~2
//~0
//~1
//~1
//~115
//~114
//~0
//~hi
//~5
//~2

// Short declarations
package main

func foo(x int) int {
	println(x)
	return x
}

func main() {
	aa, bb := 5, 0
	println(aa)
	println(bb)
	aa, bb, cc := bb, aa, 2
	println(aa)
	println(bb)
	println(cc)


	var a , b [5]int 	// pass by value
	b, zz := a, 1
	a[0] = 1
	println(b[0])
	println(zz)

	var c , d [] int 		// pass by header value, changes d
	c, xx := append(c , 0), 115
	d, ww := c, 114
	c[0] = 1
	println(d[0])
	println(xx)
	println(ww)


	var e , f struct { f int ; } 	// pass by value
	f, tt := e, "hi"
	e.f = 1
	println(f.f)
	println(tt)

	_, ss := foo(5), 2	// evaluate RHS even if result stored in blank id
	println(ss)

}