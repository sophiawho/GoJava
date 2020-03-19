// invalid index call - return type doesn't match
package test

func foo() {
	var a [12]rune;
	var b string = a[2]
}