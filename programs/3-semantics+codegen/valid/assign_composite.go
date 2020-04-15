//~hello
//~
//~3
//~0
//~0
//~1
//~0
//~

package main

func init() {
	var a, b [4]string
	b=a
	a[0] = "hello"

	type struct1 struct {
		x, y int
		z string
		aa, bb string
	}

	var c, d struct1
	c.x = 3
	d = c
	c.y = 4

	println(a[0]) // hello
	println(b[0]) // ""
	println(d.x) // 3
	println(d.y) // 0
}

func main() {
	var a, b [5]int
	b=a // Copies the contents of 'a'
	a[0]=1 // Does not change b
	
	var c, d []int
	c = append(c, 0)
	
	d = c // Copies the *header* of 'c'
	c[0] = 1 // Does change d

	type struct1 struct {
		f int
	}
	
	var e, f struct1
	
	f=e // Copies the contents of 'e'
	e.f=1 // Does not change f
	
	println(b[0]) // 0
	println(d[0]) // 1
	println(f.f) // 0
}