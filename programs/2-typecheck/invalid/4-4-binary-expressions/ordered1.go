// invalid binary exp: < unorderable type structs
package test

func foo () {
	type point struct { a,b int; }
	var x point
	var y point
	var a bool = x < y;
}