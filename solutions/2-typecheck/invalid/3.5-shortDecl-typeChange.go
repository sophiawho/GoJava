// short decl - redeclared variable given different type expression
package test

func foo() {
	var x string = "hello"
	x, y := 4, 2
}