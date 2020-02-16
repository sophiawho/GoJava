package main

func main() {
	// Valid unary op
	var x = 0; 
	x = - -3;
	// --3 isn't valid!

	// Valid bang op
	var y = false;
	b = !!!true;

	// Valid paren
	var x_ int = 3;
	x_ = (2*3);
	x_ = ((8/4) - 3);

	print(x_);
	println(x);
}
