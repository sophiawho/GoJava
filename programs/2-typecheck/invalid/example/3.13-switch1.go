// switch stmt - expr type int not compatible with case type bool
package test

func foo() {
	switch x:=1; {
	case 1:
		println(1)
	default:
		println(2)
	}
}