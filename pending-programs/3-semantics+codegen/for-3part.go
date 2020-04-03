//~0
//~1
//~

package main

func main() {
	for a, b := 0, 1; a < b; a, b = b, a {
		println(a)
		println(b)
		if (a > b) {
			continue
		}
	}
}