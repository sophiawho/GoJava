// slices - not type identical: different field names
package test

func main() {
	type s1 struct { 
		a int
	 	b float64
	}
	type s2 struct{ 
		a1 int
		b1 float64
		
	}

	var x s1
	var y s2
	x=y
}