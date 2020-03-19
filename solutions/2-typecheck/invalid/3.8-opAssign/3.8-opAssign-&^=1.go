// invalid op assignment: &^= invalid non-integer first arg
package test

func foo() {
	var a bool
	a &^= 5;
}