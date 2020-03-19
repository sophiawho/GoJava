// invalid op assignment: /= invalid second arg
package test

func foo() {
	var a int = 8
	a /= false;
}