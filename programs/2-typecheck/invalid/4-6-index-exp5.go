// invalid index call - return type doesn't match
package test

func foo() {
	var a [12]int;
	var b float64 = a[2]
}