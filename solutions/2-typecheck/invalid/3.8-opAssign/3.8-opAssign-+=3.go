// invalid op assignment: += invalid mix of args: int and float
package test

func foo() {
	var a int
	a += 5.7;
}