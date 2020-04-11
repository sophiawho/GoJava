//~true
//~true
//~true
//~true
//~true
//~2
//~appleApple
//~false
//~8 -2
//~true
//~false
//~

package main

func init() {
	// k_expKindEq
	println(true == true)
	println(1 == 1)
	println("1" == "1")
	println(1.0 == 1.0)

	var a [3]int
	var b [3]int
	println(a == b)
}

func main () {
	println(3 &^ 5)

	var a string = "apple"
	var b string = "Apple"
	println(a+b)
	println(a<b)

	println(3+5, 3-5)

	//// Boolean composition
	// k_expKindAnd
	println(true && true)
	
	// k_expKindOr
	println(false || false)
}
