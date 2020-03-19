// invalid op assignment: += invalid args
package test

func foo() {
	var a bool = true
	a += false;
}