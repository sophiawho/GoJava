package test

func init() {
	type num int
	type natural num
	var f natural = natural(num(3))
	var g string = string(num('r'))
	var h natural = natural(num('b'))
}