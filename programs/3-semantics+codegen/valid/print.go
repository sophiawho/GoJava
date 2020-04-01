// ~65
// ~66
// ~65 66
// ~6566
// ~
// ~hello
// ~hello\n

package test

func main() {
	var a rune = 'A'
	var b rune = 'B'
	println(a) // 65
	println(b) // 66
	println(a, b) // 65 66
	print(a, b) // 6566
	println()
	println()
	var c string = "hello"
	var d string = `hello\n`
	println(c)
	print(d)
}