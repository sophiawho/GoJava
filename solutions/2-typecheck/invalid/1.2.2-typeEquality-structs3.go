// slices - not type identical: different types
package test

func main() {
	type s1 struct { 
		a int
	 	b float64
	}
	type s2 struct{ 
		a int
		b int
		
	}

	var x s1
	var y s2
	x=y
}