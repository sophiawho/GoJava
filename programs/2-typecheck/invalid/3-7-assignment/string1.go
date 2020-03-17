// types don't match even if underlying types are the same
package test

type a string

func main() {
	var x string = "asd"
	var y a
	y = x
}
