// invalid index call - return type doesn't match
package test

func foo() {
	var a [12]float64;
	var b rune = a[2]
}