// valid unary expressions
package test

func foo() {
	var a int = 10
	var b float64 = 235.345
	var c rune = 'r'
	var d bool = false

	a = +a
	b = +b
	c = +c

	a = -a
	b = -b
	c = -c

	d = !d

	a = ^a
	c = ^c
}