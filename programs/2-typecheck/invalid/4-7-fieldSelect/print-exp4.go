// Field select from variable that's not of a struct type
package test

func main() {
	var a [3]int
	println(a.field)
}