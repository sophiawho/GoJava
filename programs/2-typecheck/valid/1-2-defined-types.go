package test

func main() {
	type num int
	type natural num

	var x int = 7;
	// TODO (Sophia):
	// This is failing because `num(7)` right now is a function call. 
	// We need to change `symbol` to recognize that num is a cast:
	// We can do that by getting the symbol for the identifier 'num', realizing it is mapped to 
	// a type and not a function, then changing the expression type to a cast.
	// I can fix this sometime Monday.
	var y num = num(7);
	var z natural = natural(7);

	type floats []float64
	type naturals []natural

	{
		type new []naturals
	}

	var a floats
	a = append(a, 7.2)
	var b naturals
	b = append(b, natural(8))
}