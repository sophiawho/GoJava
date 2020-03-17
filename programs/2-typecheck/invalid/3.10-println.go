// invalid println statement, b does not resolve to base type
package test

func init() {
	type a struct {
		b int
	}
	var b a
	print(b)
}