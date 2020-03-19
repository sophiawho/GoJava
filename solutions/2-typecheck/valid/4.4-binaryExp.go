// valid op assignments
package test

func foo() {
	var a bool = true || false
	a = true && false

	a = true == false
	a = 12 == 15
	a = 12.234 == 876.32
	a = "hello" == "hi"
	a = 'r' == 's'

	a = true != false
	a = 12 != 15
	a = 12.234 != 876.32
	a = "hello" != "hi"
	a = 'r' != 's'

	a = 12 < 15
	a = 12.234 < 876.32
	a = "hello" < "hi"
	a = 'r' < 's'

	a = 12 <= 15
	a = 12.234 <= 876.32
	a = "hello" <= "hi"
	a = 'r' <= 's'

	a = 12 > 15
	a = 12.234 > 876.32
	a = "hello" > "hi"
	a = 'r' > 's'

	a = 12 >= 15
	a = 12.234 >= 876.32
	a = "hello" >= "hi"
	a = 'r' >= 's'

	var b int
	var c float64
	var d string
	var e rune

	b = 1 + 2
	c = 345.235 + .234
	d = "hello " + "world"
	e = 'r' + 'e'

	b = 1 - 2
	c = 345.235 - .234
	e = 'r' - 'e'

	b = 1 * 2
	c = 345.235 * .234
	e = 'r' * 'e'

	b = 1 / 2
	c = 345.235 / .234
	e = 'r' / 'e'

	b = 1 % 2
	b = 1 | 2
	b = 1 & 2
	b = 1 << 2
	b = 1 >> 2
	b = 1 &^ 2
	b = 1 ^ 2
}