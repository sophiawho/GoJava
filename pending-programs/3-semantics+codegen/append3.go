//~!

package test

func main() {
	var a, b []int
	a = append(a, 0)
	b = a

	// 'a' and 'b' headers: len=1, cap=2, ptr=0xDEADBEEF
	a = append(a, 1)

	// b[1] is out of bounds
	b[1] = 1
}