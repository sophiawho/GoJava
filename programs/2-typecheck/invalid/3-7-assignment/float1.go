// types don't match even if underlying types are the same
package test

type a float64

func main() {
	var x float64 = 1.0
	var y a
	y = x
}
