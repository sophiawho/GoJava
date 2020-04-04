//~1
//~3 3
//~1 2
//~2 2
//~

package test

func main() {
	var a[] int
	a = append(a, 1)
	
	println(a[0])
	
	var b = append(a, 2) // PROBLEM: this reinitializes a new b
	
	b[0] = 3 // Problem: b[0] calls gets instead of puts
	
	println(a[0], b[0])
	
	println(len(a), len(b))
	println(cap(a), cap(b))
}

// Slice<Integer> __golite__b = new Slice<>();
// __golite__b.get(0) = 3;
// Slice<Integer> __golite__b = __golite__a.append(2);
// __golite__b.put(0, 3);
// Multivariable declaration