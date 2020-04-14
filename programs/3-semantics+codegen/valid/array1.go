//~true
//~false
//0
//1
//0
//0
//~true
//~false
//~1 2 0 0 1
//~

package test

func init() {
	var a, b [2] int 
	println(a==b)
	b[1] = 1
	println(a==b)

	println(b[0])
	println(b[1])
	println(a[0])
	println(a[1])
}

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
