// valid statements - function expression, return, continue, break, short decls, assignment, block, print, println, for loops, if stmts, switch, incr/decr
package test

func foo() {
	println("hi")
}

func bar() {
	foo()	
	for {
		continue
		break
	}
	return
}

func a1() int {
	return 4
}

func b1() bool {
	return true
}

// short declaration
func hello1() {
	var x int 
	var y int
	x,y,z := 1,2,3
	x,y,z = 5,6,7
	print(x,y,z)
	var a float64
	var b string
	a,b = 7.82, "hi"
	println(a,b)
}

var a int
var b int
func hello2() {
	a,b := 1,2
}

func hello3() {
	x,y,z := 1,2,3
}

// assignments
func hi1() {
	type point struct { 
		x,y,z int 
	}
	var p point
	var s []int
	var a [2]int
	p.x, s[15], a[1] = 12, 15, 22;
	println(p.x, s[15], a[1])
}

// block
func hi2() {
	var x = 2
	{
		var x = "hello world"
	}
}

// for loops
func yo1() {
	for {
		println("hello")
	}

	for true {
		println("hi")
	}

	var y = 4.2
	var z = 12
	for z==13 || y==4.2 {
		y-=1.0;
		z+=1;
	}

	// for loop - init shadowing
	var x = 12;
	for x,i:="hello",1 ; i<12; i++ {
		var x float64;
	} 
}

// if stmt - init shadowing
func yo2() {
	var x=12;
	if x,i:="hi",1; i<15 {
		var x = 15
		println("yup")
	} else {
		var x = 'r'
	}
}

// switch stmt - init shadowing
func yo3() {
	var x int = 22
	switch x,i:="hi",1; i {
	case 1,2:
			var x float64
	}
}

//increment/decrement
func yo4() {
	var x int;
	x++;
	x--;
	var y float64;
	y++;
	y--;
	var z rune;
	z++;
	z--;
}


var z struct {
	b int
}

func func1() {
	var a int
	(a) = 7
}

func func2() {
	var bool bool = true
	print(bool)
}