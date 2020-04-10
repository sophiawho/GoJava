//~true
//~false
//~

package main

func main() {
	var a [5] int
	var b [5] int
	println(a==b) // True
	
	b[0]=1
	println(a==b) // False
}