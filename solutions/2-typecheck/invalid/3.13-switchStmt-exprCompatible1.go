// switch stmt - expr not a compatible with case
package test

func foo() {
	switch x:=1; 'r' {
	case 1:
		println("won't work")
	}
}