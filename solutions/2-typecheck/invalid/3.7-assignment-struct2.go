package test

func test(){
	type s struct {
		a int;
	}
	type x s
	type y s
	var a x
	var b y
	x = y
}