//~5
//~5
//~+6.000000e+000
//~@
//~r
//~5
//~@
//~r
//~5
//~4
//~114
//~+5.000000e+000
//~+5.340000e+002
//~+9.700000e+001
//~S
//~hi
//~e
//~5
//~23
//~119
//~0
//~A
//~true
//~

package test

var b_ int

func init() {
	type num int
	type str string
	var a num = num(5)
	var b int = int(a)
	var c float64 = float64(6)
	var d string = string(64)
	var e string = string('r')
	var f rune = rune(a)
	var g str = str(d)
	var h str = str(e)

	println(int(a))
	println(b)
	println(c)
	println(d)
	println(e)
	println(f)
	println(g)
	println(h)


	var aa int = int(5)
	println(aa)
	aa = int(4.23)
	println(aa)
	aa = int('r')
	println(aa)

	var bb float64 = float64(5)
	println(bb)
	bb = float64(534.)
	println(bb)
	bb = float64('a')
	println(bb)

	var cc string = string(83)
	println(cc)
	cc = string("hi")
	println(cc)
	cc = string('e')
	println(cc)

	var dd rune = rune(5)
	println(dd)
	dd = rune(23.)
	println(dd)
	dd = rune('w')
	println(dd)
}

func main() {
	println(b_)
	var a int = 65
	println(string(a))

	var c bool = bool(true)
	println(c)
}