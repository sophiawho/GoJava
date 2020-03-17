// attempt to op assign to nonaddressable variable
package test

func main() {
    var a int = 1
    1 += a
}

