// invalid field selection - id is not a field
package test

func foo() {
	type structType struct { 
		x,y int 
	}
	var structElem structType
	var a = structElem.val
}