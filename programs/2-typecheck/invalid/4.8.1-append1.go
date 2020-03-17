// invalid append: second expression has wrong type
package test

func foo() {
	var a []int
	b := append(a, 5.6)
}