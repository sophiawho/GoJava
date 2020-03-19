// slices - not type identical: different sequence of fields
package test

func main() {
	type s1 struct { 
		a int
	 	b float64
	}
	type s2 struct{ 
		b float64
		a int
	}

	var x s1
	var y s2
	x=y
}