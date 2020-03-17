// []int is not assignment compatible with a in assign statement
package test

func test(){
	type a []int
	var y []int
	var x a
	x = y
}
