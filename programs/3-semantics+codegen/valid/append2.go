//~Cap: 2 , len: 1
//~Cap: 2 , len: 1
//~Cap: 2 , len: 2
//~Cap: 2 , len: 1
//~0 1 0
//~

package test

func main() {
	var a []int
	var b []int
	a = append(a, 0)
	b = a

	println("Cap:", cap(a), ", len:", len(a))
	println("Cap:", cap(b), ", len:", len(b))
	// 'a' and 'b' headers: len=1, cap=2, ptr=0xDEADBEEF
	a = append(a, 1)

	println("Cap:", cap(a), ", len:", len(a))
	println("Cap:", cap(b), ", len:", len(b))

	println(a[0], a[1], b[0])
}