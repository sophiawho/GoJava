// invalid print statement, a does not resolve to base type
package test

func init() {
	var a []int 
	print(a)
}