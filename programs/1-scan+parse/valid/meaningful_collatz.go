// Print the Collatz sequence
package main

func main() {
	var collatz = 6;
	println("Printing out the Collatz sequence of number: ", collatz);
	printCollatz(collatz);
}

func printCollatz(collatz int) {
	for {
		println(collatz);
		if (n & 1 == 1) {
			collatz = 3*collatz+1;
		} else {
			collatz = collatz / 2;
		}
		if (collatz == 1) {
			break;
		}
	}
	println(collatz);
	return;
}