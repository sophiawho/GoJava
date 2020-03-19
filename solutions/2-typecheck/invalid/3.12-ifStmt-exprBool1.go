// if stmt - expr is not boolean
package test

func foo() {
	if x:=1; 'r' {
		println("won't work")
	}
}