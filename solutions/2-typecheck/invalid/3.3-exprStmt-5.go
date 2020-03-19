// invalid len: return type doesn't match assignment type
package test

func foo() {
	var a [5]int
	var b string = len(a)
}
