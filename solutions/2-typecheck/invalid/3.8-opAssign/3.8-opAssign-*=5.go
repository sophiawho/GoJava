// invalid op assignment: *= invalid args bool
package test

func foo() {
	var a bool = true
	a *= false;
}