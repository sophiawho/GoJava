// switch stmt - expr not compatible with case
package test

func foo() {
	switch x:=1; {
	case true:
		println(1)
	case false:
		print(3)
	default:
		x++
	}

	switch x:=1; x {
	case 3:
		println(1)
	case 2:
		print(3)
	default:
		x++
	}
}