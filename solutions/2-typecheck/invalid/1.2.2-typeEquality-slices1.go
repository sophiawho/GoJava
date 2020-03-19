// slices - not type identical: differing base types
package test

func main() {
	var a []int
	var b []string
	a=b
}