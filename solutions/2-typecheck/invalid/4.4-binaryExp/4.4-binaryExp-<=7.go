// invalid binary exp: <= mismatch types rune and int
package test

func foo () {
	var x int = 5
	var y rune = 'r'
	var a bool = x <= y;
}