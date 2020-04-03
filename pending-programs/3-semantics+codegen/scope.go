//~3
//~3
//~

package test

func main() {
	var a int = 3
	{
		var b int = a
		var a int = a
		fmt.Println(a)
		fmt.Println(b)	
	}
}
