package test 

type a int
type b a

func bar() a {
	var bb b
	return bb
}