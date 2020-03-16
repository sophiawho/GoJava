// invalid type cast : type and expr doesn't resolve to identical underlying types
package test

func foo() {
	type num int
	var someString string

	var y num = 3
	var x = num(someString)
}