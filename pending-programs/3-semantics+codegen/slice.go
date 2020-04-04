//~1
//~3 3
//~1 2
//~2 2
//~

package test

func main() {
	var a[] int
	a = append(a, 1)
	
	println(a[0])
	
	var b = append(a, 2)
	
	b[0] = 3
	
	println(a[0], b[0])
	
	println(len(a), len(b))
	println(cap(a), cap(b))
}
