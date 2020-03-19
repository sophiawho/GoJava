// invalid binary exp: >= mismatch types rune and string
package test

func foo () {
	var x rune = 'g'
	var y string = "hello"
	var a bool = x >= y;
}