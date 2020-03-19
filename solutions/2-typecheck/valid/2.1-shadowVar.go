// valid shadowing of variables
package test

var x int = 7

func main() {
	var x int
	if x==7 {
		var x int = 12
	} else {
		var x int = 15
	}
}