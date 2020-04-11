//~true
//~false
//~1 2 0 0 1
//~

package test

func init() {
	var a [5] int
	var b [5] int
	println(a==b) // True
	
	b[0]=1
	println(a==b) // False
}

func main() {
	var a [5]int
	a[0] = 1
	a[1] = 2
	a[4] = a[0]

	println(a[0], a[1], a[2], a[3], a[4])
}
