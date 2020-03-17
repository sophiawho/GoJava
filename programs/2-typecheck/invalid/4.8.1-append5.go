// invalid append: second expression has wrong type
package test

func foo() {
	var a []string
	b := append(a,'r')
}