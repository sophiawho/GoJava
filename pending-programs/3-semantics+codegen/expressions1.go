//~true
//~false
//~true
//~true
//~true
//~true
//~false
//~false
//~false
//~false
//~true
//~true
//~false
//~false
//~true
//~true
//~false
//~false
//~2
//~+2.000000e+000
//~11
//~0
//~+0.000000e+000
//~5
//~5
//~2
//~+2.500000e+000
//~0
//~+0.500000e+000
//~-0.500000e+000
//~1
//~+1.000000e+000
//~-1.000000e+000
//~4
//~4
//~0
//~0
//~0
//~1
//~1
//~+1.000000e+000
//~-1
//~-1.000000e+000
//~false
//~-2
//~1
//~1
//~420

package main

func main() {
	
	// Binary Operators

	//// Boolean composition
	// k_expKindAnd
	println(true && true)
	
	// k_expKindOr
	println(false || false)
	
    //// Relational binary operators
	// k_expKindEq
	println(true == true)
	println(1 == 1)
	println("1" == "1")
	println(1.0 == 1.0)

	// k_expKindNotEq
	println(true != true)
	println(1 != 1)
	println("1" != "1")
	println(1.0 != 1.0)

	// k_expKindLess
	println(1 < 2)
	println(1.0 < 2.0)
	
	// k_expKindLessEq
	println(2 <= 1)
	println(2.0 <= 1.0)
	
    // k_expKindGrtr
	println(2 > 1)
	println(2.0 > 1.0)
	
	// k_expKindGrtrEq
	println(1 >= 1)
	println(1.0 >= 2.0)
	
    //// Additive binary operators
	// k_expKindAdd
	println(1 + 1)
	println(1.0 + 1.0)
	println("1" + "1")
	
	// k_expKindMinus
	println(1 - 1)
	println(1.0 - 1.0)
	
	// k_expKindBitOr
	println(4|1)
	
	// k_expKindBitXOR
	println(4^1)

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
	println(1.0 % 2.)
	println(-1.0 % 2.)

	// k_expKindLeftShift
	println(1 << 2)
	println(1.0 << 2)
	
	// k_expKindRightShift
	println(1 >> 2)
	println(1.0 >> 2)
	
	// k_expKindBitAnd
	println(4&1)

	// k_expKindBitClear
	println(1&^2)
	
    //// Golite supports four unary operators
	// k_expKindUPlus
	println(+1)
	println(+1.0)
	
    // k_expKindUMinus
	println(-1)
	println(-1.0)

	// k_expKindBang
	println(!true)

	// k_expKindUBitXOR // ATTN: This is similarly named to binary op k_expKindBitXOR
	println(^1)
	
	// k_expKindFuncCall
	
	// k_expKindArrayAccess
	var a [2]int
	a[0] = 1
	a[1] = a[0]
	println(a[0])
	println(a[1])

    // k_expKindFieldAccess
	type mystruct struct {
		x int
	}
	var b mystruct
	b.x = 420
	println(b.x)

	// Builtins
	
}