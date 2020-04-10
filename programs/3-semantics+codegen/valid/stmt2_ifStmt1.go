//~a == 0
//~b != 0
//~true
//~d equals 0
//~Hello
//~Something
//~World
//~f() called
//~b == 2
//~b = 10

package main

func f() {
	println("f() called")
}

func main() {
	var a int = 0

	if a == 0 {
		println("a == 0")
	} else {
		println("a != 0")
	}

	var b int = 1

	if b == 0 {
		println("b == 0")
	} else {
		println("b != 0")
	}
	
	// Short var decl
	if c := "Hello"; true {
		if false {
			println("Wrong")
		} else {
			if true {
				println("true")
			}
			if false {
				
			} else if d := 0; d == 0 {
				print("d equals ")
				println(d)
				println(c)
			} else {
				println("Wrong")
			}
			println("Something")
		}
	}

	c := "World"
	println(c)

	// EmptyStmt
	if ; false {
	}

	// ExpressionStmt
	if f(); false {
	}

	// IncDecStmt
	if b++; b == 2 {
		println("b == 2")
	}

	if b = 10; true {
		println("b = 10")
	}


}