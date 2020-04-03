//~0
//~1
//~0
//~

package main

func main() {
	var a, b [5]int
	b=a // Copies the contents of 'a'
	a[0]=1 // Does not change b
	
	var c, d []int
	c = append(c, 0)
	
	d = c // Copies the *header* of 'c'
	c[0] = 1 // Does change d
	
	var e, f struct {f int;}
	
	f=e // Copies the contents of 'e'
	e.f=1 // Does not change f
	
	println(b[0])
	println(d[0])
	println(f.f)
}