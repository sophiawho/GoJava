// invalid len: return type doesn't match assignment type
package test

func foo() {
	var a []int
	var b rune = len(a)
}