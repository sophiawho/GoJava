// invalid append: first expression not a slice
package test

func foo() {
	var a string
	b := append(a,"hello")
}