// invalid type cast: string cast of non-integer expression type
package test

func foo() {
	var x = string(0.01)
}