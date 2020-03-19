// for loop - expr is not boolean
package test

func foo() {
	for "hello" {
		println("won't work")
	}
}