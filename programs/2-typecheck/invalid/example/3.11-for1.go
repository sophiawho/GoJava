// expr condition in for statement is not type bool
package test

func main() {
	for i := 0; i; i++ {
		print(i)
	}
}
