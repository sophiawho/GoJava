//~true
//~false
//0
//1
//0
//0

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
