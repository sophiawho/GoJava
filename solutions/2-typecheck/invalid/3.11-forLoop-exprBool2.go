// for loop - expr is not boolean
package test

func foo() {
	for i:=3; 'p'; i++ {
		println("won't work")
	}
}