//~3
//~

package test

// if_init.java:101: error: variable __golite__a is already defined in method __golite__main()
func main() {
	if a := false; a {
	} else if a := true; !a {
	} else if a {
		println("3")
	} else {
		println("4")
	}
}