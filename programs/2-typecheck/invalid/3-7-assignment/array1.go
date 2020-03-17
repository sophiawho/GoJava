// [2]int is not assignment compatible with a in assign statement
package test

func test(){
	type a [2]int
	var y [2]int
	var x a
	x = y
}