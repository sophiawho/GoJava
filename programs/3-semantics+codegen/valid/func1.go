//~f1 called
//~f8 called
//~1 2 +3.000000e+000 4 5 6 hello
//~f9 called
//~1 +4.000000e+000
//~

package main

func f1() {
	println("f1 called")
}

func f2() int {
	return 2
}

func f3() float64 {
	return 3.0
}

func f4() [3]int {
	var a [3]int
	return a
}

func f5() []int {
	var a []int
	return a
}

type struct1 struct {
	x int
}

func f6() struct1 {
	var a struct1
	a.x = 1
	return a
}

func f7() string {
	return "f7"
}

func f8(x1, x2 int, x3 float64, x4 [3]int, x5 []int, x6 struct1, x7 string) {
	println("f8 called")
	println(x1, x2, x3, x4[0], x5[0], x6.x, x7)
}

func f9(x1, _ int, _, x2, _ float64, _ int) {
	println("f9 called")
	println(x1, x2)
}

// type indirect_struct1 struct1

// Bug: Slice disappears
// func f10(x1 [3][]int, x2 [][3]int) {
// 	println("f10 called")
// } 

// func f11(x1 indirect_struct1, x2 [3][]indirect_struct1) {
// 	println("f11 called")
// }

func main() {
	f1()
	f2()
	f3()
	f4()
	f5()
	f6()
	f7()
	var a [3]int
	a[0] = 4
	var b []int
	b = append(b, 5)
	var c struct1
	c.x = 6
	f8(1, 2, 3.0, a, b, c, "hello")
	f9(1, 2, 3.0, 4.0, 5.0, 6)
}