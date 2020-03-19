// valid shadowing of type declarations
package test

type num int
var x num = num(7)

func main() {
	type num float64
	var y num = num(5.3)
}