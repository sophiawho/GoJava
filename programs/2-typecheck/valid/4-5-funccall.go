package main

func foo (a rune) {
	print(true)
}

func main() {
	var b rune
	var a rune
	(foo)(a)
	(foo)(b)
	foo(b)
}
