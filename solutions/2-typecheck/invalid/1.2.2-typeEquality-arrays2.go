// arrays - not type identical: differing base types
package test

func main() {
	var a [12]int
	var b [12]string
	a=b
}