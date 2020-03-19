// invalid append: return type doesn't match assignment type
package test

func foo() {
	var a []int
	var b int = append(a,4)
}