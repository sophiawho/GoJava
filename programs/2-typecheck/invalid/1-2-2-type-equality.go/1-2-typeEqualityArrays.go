// Return type expects an array of different size
package test 

type a int

func bar() [5]int {
	var bar [4]int
	return bar
}