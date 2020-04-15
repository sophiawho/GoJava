package test

func main() {
	var a, b = 3, 2
	a, b, c := b, a, 4 // Error: (line 7) Void cannot be used as a value in variable declaration.
	println(a, b, c)
}