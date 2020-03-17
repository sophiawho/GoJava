// expr condition in else if statement is not type bool
package test

func main() {
	var int c = 2
	if c < 2 {
		print(3)
	} else if c {
		print(4)
	}
}
