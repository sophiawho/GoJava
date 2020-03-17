// Field select from variable that's not of a struct type
package test

func main() {
	var a int = 5;
	println(a.field)
}