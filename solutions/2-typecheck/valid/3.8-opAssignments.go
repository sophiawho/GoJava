// valid op assignments
package test

func foo() {
	var a bool
	var b int
	var c float64
	var d string

	b += 1
	c += 345.235
	d += "hello "

	b -= 1
	c -= 34.4

	b *= 1
	c *= 3.5

	b /= 4
	c /= 4.34

	b %= 5
	b |= 1
	b &= 5
	b <<= 2
	b >>= 4
	b &^= 6
	b ^= 9 
}