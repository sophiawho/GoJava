package main

func _() int {return 0;}
func _() {return;}
func f1() []int {
	var a []int
	return a
}
func f2() [3]int {
	var a [3]int
	return a
}
func f3() int {
	var a int
	return a
}

type p struct {
	x int
	_ int
}

func f4() p {
	var a p
	return a
}

func main() {
	var a = 1
	var b = 2
	var _ int = 1
	print(a)
	println(b)
	f1()
	f2()
	f3()
	f4()
	var c, d, _ int = 1, 2, 3
	for i := 0; i < 5; i++ {
		continue
	}
	for i := 0; i < 5; i++ {
		break
	}
}

