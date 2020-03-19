// Scoping oddity
package test

func main() {
	// scoping
	var hi = "hi"
	{
		var hello = hi
		var hi = hi
	}
}