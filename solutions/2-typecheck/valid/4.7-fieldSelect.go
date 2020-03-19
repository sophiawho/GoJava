// valid field selection
package test

func foo() {
	type structType struct {
		a int
		b float64
		c rune
		d string
		e [5]int
	}
	type structType2 structType
	var structElem structType

	var e, e2 int = structElem.a, structElem.a
	var f float64 = structElem.b
	var g rune = structElem.c
	var h string = structElem.d
	var i [5]int = structElem.e
	var j int = structElem.e[2]
}

/*
{
	int [type] = int
	float64 [type] = float64
	bool [type] = bool
	rune [type] = rune
	string [type] = string
	true [constant] = bool
	false [constant] = bool
	{
		foo [function] = () -> void
		{
			structType [type] = structType -> struct { a int; b float64; c rune; d string; e [5]int; }
			structType2 [type] = structType2 -> structType -> struct { a int; b float64; c rune; d string; e [5]int; }
			structElem [variable] = structType
			e2 [variable] = int
			e [variable] = int
			f [variable] = float64
			g [variable] = rune
			h [variable] = string
			i [variable] = [5]int
			j [variable] = int
		}
	}
}
*/
