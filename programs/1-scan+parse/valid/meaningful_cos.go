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
		if (addend < 0.00005) {
			return sum;
		}
		if (iterationNumber % 2 == 0) { // Even iteration
			sum += addend;
		} else { // Odd iteration
			sum -= addend;
		}
		iterationNumber++;
	}

}

func power(base float64, exp float64) float64 {
	var result = base
	i := 0
	for i; i < exp - 1; i++ {
		result *= base
	}
	return result
}

func factorial(x int) int {
	var result = 1
	i := 1
	for i; i <= x; i++ {
		result *= i 
	}

	return result
}