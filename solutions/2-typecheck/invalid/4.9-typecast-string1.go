// invalid type cast : string cast of non-integer/rune type 
package test

func foo() {
	var x = string(6.345)
}