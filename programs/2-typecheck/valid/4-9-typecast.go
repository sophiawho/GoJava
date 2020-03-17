package test

func foo() {
	var x = string('a')
	var y = string(3)
	var z = rune(4)
}

func init() {
	type num int
	type natural num
	var foo int = 3
	var bar = num(foo)
	var a num = num(5)
	var d natural = natural(a+num(7))
	var c natural = natural(d)

	var dd int = int(4.72)
	var ee float64 = float64(3)

	var f string = string(456)
	var g string = string('r')
}