// invalid type cast: int is numeric, expr is string
package test

func foo() {
	var x = int("test")
}