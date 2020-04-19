//~30
//~-1
//~1
//~2
//~0
//~

package main

func main() {
	var a int = 5
	a *= 6 // essentially a = a * 6
	println(a) // 30
	
	var b [2]int
	b[0] -= 1
	println(b[0]) // -1
	
	b[1] -= b[0]
	
	println(b[1]) // 1
	
	a &= 3
	println(a) // 2
	
	a &^= 2
	println(a) // 0
}
