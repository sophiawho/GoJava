// init does not introduce a binding and may be declared multiple times

package main

func main() {
}

func init() {
    var a int
}

func init() {
    var a int
}

