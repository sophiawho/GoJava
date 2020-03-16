// invalid index call - index not of type int
package test

func foo() {
	var a [12]int;
	var b = a["hello"]
}