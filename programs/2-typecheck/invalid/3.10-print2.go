// invalid print statement, a does not resolve to base type
package test

func init() {
	var a []int 
	var b rune
	var c string
	print(c, b, a)
}