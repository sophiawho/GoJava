package main

func foo (a rune, b int, c string) {
	print(true)
}

func bar(a []rune) {
	print(false)
}

func main() {
	var c rune
	var d int
	var e string
	(foo)(c, d, e)
	(foo)(c, d, e)
	foo(c, d, e)
}

func init() {
	var f []rune
	bar(f)
}