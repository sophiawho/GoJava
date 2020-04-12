//~case 8, 9
//~default
//~case "Two" >= y
//~case x == 2.0
//~case x1 == 1
//~1
//~10
//~42
//~6
//~case true
//~case 1 == 2
//~1
//~1
//~case 1, 2
//~2
//~case 1, 2
//~3
//~break
//~3
//~

package main

type mystruct struct {
	x int
}

func f1() bool {
	return true
}

func main() {
	var tag = 9
	var nine = 9
	switch tag {
	default: 	
		println("default")
	case 0, 1, 2, 3: 
		println("case 0, 1, 2, 3")
		break
	case 4, 5, 6, 7: 
		println("case 4, 5, 6, 7")
		break
	case 8, 9:
		println("case 8, 9")
	case nine, 10:
		println("case nine, 10")
	}
		
	switch x := "Hello"; {  // missing switch expression means "true"
	case x == "hello": 
		println("case x == \"hello\"")
		break
	default: println("default")
	}
		
	switch x, y, z := 1.0, "Two", 3 ; f1() {
	case x == 2.0: 
		println("case x == 2.0")
	case "Two" >= y: 
		println("case \"Two\" >= y")
	case z == 4: 
		println("case z == 4")
	default: println(4)
	}

	switch x, y := 1.0, "Two" ; false {
	case x == 2.0: 
		println("case x == 2.0")
	case "Two" >= y: 
		println("case \"Two\" >= y")
	default: println(4)
	}

	var x1 = 0
	var x2 [5]int
	x2[0] = 1
	var x3 []int
	x3 = append(x3, 1)
	var x4 mystruct
	x4.x = 3
	
	switch x1, x2[0], x4.x = 1, x2[0]+9, x4.x + 3; {
	case x2[0] == 3: 
		println("x2[0] == 3")
	case x3[0] == 2: 
		println("case x3[0] == 2")
	case x4.x == 1:
		println("case x4.x = 1")
	case x1 == 1: 
		x3[0] = 42
		println("case x1 == 1")
	default: println(4)
	}
	println(x1)
	println(x2[0])
	println(x3[0])
	println(x4.x)

	switch i1 := 1; {
		case false: println("case false")
		case true:
			println("case true")
			switch i2 := i1; false {
				case 1 == 1: 
					println("case 1 == 1")
				case 1 == 2: 
					println("case 1 == 2")
					println(i2)
					for ; i1 < 5; i1++ {
						println(i1)
						switch i1 {
							case 1, 2:
								println("case 1, 2")
								if (true) {
									break
								}
							default:
								println("break")
								break
						}
						if (i1 == 3) {
							break
						} else {
							continue
						}
					}
			}
			println(i1)
	}
}