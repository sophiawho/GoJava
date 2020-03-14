package main
func f1() []int {
	var a []int
	return a
}
func main() {
	f1()[1] = 2
}
