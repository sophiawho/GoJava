// valid type casts
package test

func foo() {
	type num int
	type natural num

	var a num = num(5)
	var b natural = natural(a+num(7))
	var c natural = natural(12)

	var d int = int(5.72)
	var e float64 = float64(4)

	var f string = string(456)
	var g string = string('r')
}