// invalid append: second expression has wrong type
package test

func foo() {
	var a []int
	a = append(a, 5.1)
}
