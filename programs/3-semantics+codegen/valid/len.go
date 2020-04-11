//~4
//~0
//~2
//~5
//~2
//~4
//~0
//~

package main

func init() {
	var a = "Test"
	var b = ""
	println(len(a))
	println(len(b))
}

func init() {
	var b []int
	b = append(b, 1)
	b = append(b, 2)
	println(len(b))
}

func main() {
	var a [5]int
	println(len(a))

	var b []int
	b = append(b, 1)
	b = append(b, 2)
	println(len(b))

	println(len("Test"))
	println(len(""))
}
