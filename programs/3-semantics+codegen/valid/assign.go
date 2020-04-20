//~0 Hey 0
//~0
//~1
//~0
//~

package main

func init() {
	type e struct {
		c string
		b[1] int
		cc[3] string
	}
	var c e
	c.c = "Hey"
	println(c.b[0], c.c, len(c.cc[0])) // 0 Hey 0
}

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
	
	println(b[0]) // 0
	println(d[0]) // 1
	println(f.f) // 0
}