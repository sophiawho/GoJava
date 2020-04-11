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
//~2 3
//~2 3
//~2 3
//~

package test

func main() {
    // k_loopKindThreePart
	var a []int
	for i:=0; i<10; i++ {
		println("Cap:", cap(a), ", len:", len(a))
		a = append(a, 0)
	}

	for x1, x2, x3 := 1, 2, 3 ; x1 < 3 ; x1++ {
		println(x2, x3)
	}

	for x1, x2, x3 := 1, 2, 3 ; x1 < 3 ; x1, x2, x3 = 3, 2, 1 {
		println(x2, x3)
	}
	// x1, x2, x3 := 1, 2, 3
	// x1, x2, x3 = 3, 2, 1
}
