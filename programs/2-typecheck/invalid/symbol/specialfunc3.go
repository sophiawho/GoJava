// init does not introduce a binding and may be declared multiple times
// main can therefore not call init

package main

func main() {
	init()
}

func init() {
    var a int
}
