// invalid type cast : type and expr aren't both numeric
package test

func foo() {
	var x = int("word")
}