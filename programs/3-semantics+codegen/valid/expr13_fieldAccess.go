//~420

package main

func main() {

    // k_expKindFieldAccess
	type mystruct struct {
		x int
	}
	var b mystruct
	b.x = 420
	println(b.x)
}
