// valid type equality
package test

func main() {
	type num int

	// arrays
	var a [12]int
	var b [12]int
	var c [12]num
	a=b
	b[2]=int(c[3])
	c[5]=num(a[5])

	// slices
	var d []int
	var e []int
	var f []num
	d=e
	e[4]=int(f[1])
	f[8]=num(d[2])

	// struct
	type s1 struct { 
		a int
	 	b float64
	}
	type s2 struct{ 
		a int
		b float64
		
	}
	var x s1
	var y s2
	x.a=y.a
	y.b=x.b

	// base types
	var g int
	var h int
	g=h

	var i float64
	var j float64
	i=j

	var k string
	var l string
	k=l

	var m rune
	var n rune
	m=n

	var o bool
	var p bool
	o=p
}