package test

func main() {
	type num int
	type natural num

	var x int = 7;
	var y num = num(7);
	var z natural = natural(7);

	type floats []float64
	type naturals []natural

	{
		type new []naturals
	}

	// TODO Implement append
	var a floats
	a = append(a, 7.2)
	var b naturals
	b = append(b, natural(8))
}