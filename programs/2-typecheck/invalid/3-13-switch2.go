// switch stmt - expr not compatible with case
package test

func foo() {
	switch x:=1; x {
	case 1:
		println(1)
	case 'a':
		print(3)
	default:
		x++
	}
}