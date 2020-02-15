package main

func main() {
	// Valid integer expressions
	var x int = 0;
	var y = 0;
	
	x = 2 + 3;
	x = 2 - 3;
	x = 2 * 3;
	x = 2 / 3;
	x = 2 % 3;
	x = -2;
	
	// FP operations
	var y float = 0.0;
	y = 0.0 + 0.0;
	y = .1 - .2;
	y = .3 * 4.2;
	y = -4.
	
	// string operations
	var s = "hi"
	s = "string" + "string";
	s = "string" * 3;
	
	// valid logical operators
	var b bool = false;
	var z bool = true;
	b = z == 0;
	b = z != 0;
	b = b && z == 0;
	b = b || z != 0;
	b = z > 0;
	b = z >= 0;
	b = z <= 0;
	b = z < 0;
}
