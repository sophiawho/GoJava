// invalid index call - return type doesn't match
package test

func foo() {
	var a [12]string;
	var b int = a[2]
}