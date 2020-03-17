// Field select from variable that's not of a struct type
package test

func main() {
	var a []int
	println(a.field)
}