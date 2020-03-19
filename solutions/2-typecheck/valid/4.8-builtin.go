// valid built in functions
package test

func foo() {
	// append
	var a []int
	var b []float64
	var c []rune
	var d []string

	a = append(a, 5)
	b = append(b, .634)
	c = append(c, 'e')
	d = append(d, "hello")

	//cap
	var e [12]int
	var f int = cap(a)
	var g int = cap(b)
	var h int = cap(e)

	//length
	var i int = len(c)
	var j int = len(d)
	var k int = len(e)
	var l int = len("hello")
}