//~0 1 0
//~

package main

var a [5] int
var b []int
var c struct { f int; }

func foo() [5]int { return a; }
func bar() []int { 
	b = append(b, 0)
	return b; 
}
func baz() struct{ f int; } { return c; }

func main () {
	var d, e, f = foo(), bar(), baz()
	d[0] = 1
	e[0] = 1
	f.f = 1 
	println(a[0], b[0], c.f)
}
