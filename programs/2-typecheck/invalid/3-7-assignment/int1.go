// types don't match even if underlying types are the same
package test

type a int

func main() {
	var x int = 1
	var y a
	y = x
}
