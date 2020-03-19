// invalid unary expression: ! invalid rune arg
package test

func foo() {
	var a rune
	println(!a)
}