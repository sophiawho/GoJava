// invalid len: return type doesn't match assignment type
package test

func foo() {
	var a string
	var b string = len(a)
}