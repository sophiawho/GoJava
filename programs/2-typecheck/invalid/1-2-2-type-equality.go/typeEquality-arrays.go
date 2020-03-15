package test 

type a [12]int
type b [11]int

func foo() a {
	var bar a
	return bar
}