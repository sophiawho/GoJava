// invalid op assignment: &= invalid non-integer second arg
package test

func foo() {
	var a int
	a &= 5.22;
}