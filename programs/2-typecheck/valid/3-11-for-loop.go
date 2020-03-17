package test

func main() {
	for i := 0; i < 5; i++ {
		print(i)
	}
	for i := 0; i < 5; i++ {
		print(i)
	}
	for i := 0; i < 5; i-- {
		var i int // redeclare variable declared in init statement
		print(i)
	}
	var a bool = false
	for a {

	}
}
