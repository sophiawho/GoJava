// invalid use of function before declaration
package test

var x int = firstValue(23, 24)

func firstValue(a, b int) int {
	return a
}