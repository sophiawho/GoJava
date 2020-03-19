// invalid type cast : type and expr doesn't resolve to identical underlying types
package test

func foo() {
	type num int
	type notNum string

	var x = num(notNum)
}