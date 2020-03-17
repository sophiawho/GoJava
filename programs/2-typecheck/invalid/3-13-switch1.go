// switch stmt - expr not compatible with case
package test

func foo() {
	switch x:=1; 'r' {
	case 1:
		println(1)
	default:
		println(2)
	}
}