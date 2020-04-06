//~1
//~+1.000000e+000
//~-1
//~-1.000000e+000
//~false
//~-2

package main

func main() {
	
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
}
