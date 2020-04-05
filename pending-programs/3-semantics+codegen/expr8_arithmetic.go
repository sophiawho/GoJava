//~2
//~+2.000000e+000
//~11
//~0
//~+0.000000e+000
//~2
//~+2.500000e+000
//~0
//~+0.500000e+000
//~-0.500000e+000
//~1
//~2
//~0

package main

func main() {
	
    //// Additive binary operators
	// k_expKindAdd
	println(1 + 1)
	println(1.0 + 1.0)
	println("1" + "1")
	
	// k_expKindMinus
	println(1 - 1)
	println(1.0 - 1.0)
	
    //// Multiplicative binary operators
	// k_expKindMult
	println(1 * 2)
	println(1.0 * 2.5)

	// k_expKindDiv
	println(1/2)
	println(1.0/2.)
	println(-1.0/2.)

	// k_expKindMod
	println(1 % 2)
	
	// k_expKindIncDec
	println(1++)
	println(1--)
}
