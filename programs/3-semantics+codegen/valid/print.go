//~255 255 +1.200000e-001 76
//~
//~65
//~66
//~65 66
//~6566
//~
//~hello
//~hello\n

package test

func init() {
	println(255, 0377, 0.12, 'L')
}

func init() {
	println();
}

func init() {
	var a rune = 'A'
	var b rune = 'B'
	println(a) // 65
	println(b) // 66
	println(a, b) // 65 66
	print(a, b) // 6566
	println()
	println()
}

func main() {
	var c string = "hello"
	var d string = `hello\n`
	println(c)
	print(d)
}