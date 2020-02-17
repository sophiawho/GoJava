package main

type p1 struct {
	x int
}

type p2 struct {
	p p1
}

func main() {
	
	println(f1().x)
	println(f2()[0].x)
	println(f3()[0].x)
	
	var a1 [1]p2
	var a2 [1][1]int
	a2[0][0] = 0
	
	println(a1[a2[0][0]].p.x)
}

func f1() p1 {
	var a p1 
	a.x = 41
	return a
}

func f2() [1]p1 {
	var a [1]p1
	// a[0] = p1{42}
	return a
}

func f3() []p1 {
	// var a []p1 = []p1{{43}}
	return a
}
