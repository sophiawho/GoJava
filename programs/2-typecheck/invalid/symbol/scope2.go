// out of scope variable access

package test

func main() {
	{
		var b int = 3
	}
	print(b)
}