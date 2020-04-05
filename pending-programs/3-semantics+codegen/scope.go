//~3
//~3
//~

package test

func main() {
	var a int = 3
	{
		var b int = a
		var a int = a
		println(a)
		println(b)
	}
}
