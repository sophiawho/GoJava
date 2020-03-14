// out of scope type declaration

package test

func main() {
	{
		type b int
	}
	type c b
}