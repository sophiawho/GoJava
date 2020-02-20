// Approximate value of e by using Maclaurin series expansion
package main

func main() {
	var sum float64 = 0.0
	var last float64 = 0.0
	
	var factorial float64 = 1.0
	var threshold float64 = 0.001
	
	var change = 1.0
	
	for n := 1.0; change > threshold; n++ {
		last = sum
		sum = sum + 1 / factorial

		factorial = factorial * n
		
		change = sum - last
	}
	println("Approximate value of e:")
	println(sum)
}


func power(base, exp int) int {
	var result = base
	for i := 0; i < exp - 1; i++ {
		result *= base
	}
	return result
}
