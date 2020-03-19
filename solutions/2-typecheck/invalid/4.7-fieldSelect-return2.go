// invalid field selection - return type doesn't match
package test

func foo() {
	type structType struct { 
		x,y int 
	}
	var structElem structType
	var a string = structElem.x
}