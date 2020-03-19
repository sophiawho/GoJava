// function declaration - invalid redeclaration of formal parameter
package test

func f1(a int, a float64) {
	var a int
}
