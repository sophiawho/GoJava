//~Cap: 0 , len: 0
//~Cap: 2 , len: 1
//~Cap: 2 , len: 2
//~Cap: 4 , len: 3
//~Cap: 4 , len: 4
//~Cap: 8 , len: 5
//~Cap: 8 , len: 6
//~Cap: 8 , len: 7
//~Cap: 8 , len: 8
//~Cap: 16 , len: 9
//~Cap: 5 , len: 5
//~

package test

func main() {
	var a []int
	println("Cap:", cap(a), ", len:", len(a))

	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))

	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))
	
	a = append(a, 0)
	println("Cap:", cap(a), ", len:", len(a))

	var b[5]int
	println("Cap:", cap(b), ", len:", len(b))
}