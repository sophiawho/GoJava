// Approximate value of sin(angle) by Maclaurin series expansion
package main

func main() {
	println(sin(3.14, 4))
}


func power(base float64, exp float64) float64 {
	var result = base
	for i := 0.0; i < exp - 1; i++ {
		result *= base
	}
	return result
}

func sin(x, num_iters float64) float64 {
	
	var n = 0.0
	var numerator = 1.0
	var denominator = 1.0
	var sum = x
	
	for n < num_iters {
		n++
		numerator = power(-1, n)
		denominator = float64(factorial(2 * int(n) + 1))
		sum += numerator / denominator * power(x, 2 * n + 1)
		
	}
	return sum
} 

func factorial(x int) int {
	var result = 1
	for i := 1; i <= x; i++ {
		result *= i 
	}

	return result
}