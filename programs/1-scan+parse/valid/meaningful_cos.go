// Approximate value of cos(angle) by Maclaurin series expansion
package main

func main() {
	var angle = 0.5;
	print("The approximate value of cos(", angle, ") is: ")
	println(computeCos(angle));
}

func computeCos(angle float64) float64 {
	var n int = 0
	var sum float64 = 1
	var iterationNumber = 1;

	for {
		var numerator = power(numerator, iterationNumber*2);
		var denominator = factorial(iterationNumber*2);
		var addend = numerator/float64(denominator);
		if (iterationNumber % 2 == 0) { // Even iteration
			sum += addend;
			} else { // Odd iteration
				sum -= addend;
			}
			iterationNumber++;
			if (addend < 0.00005) { // Stopping condition
				return sum;
			}
		}
	
	return sum;
}

func power(base float64, exp float64) float64 {
	var result = base
	for i := 0; i < exp - 1; i++ {
		result *= base
	}
	return result
}

func factorial(x int) int {
	var result = 1
	for i := 1; i <= x; i++ {
		result *= i 
	}

	return result
}