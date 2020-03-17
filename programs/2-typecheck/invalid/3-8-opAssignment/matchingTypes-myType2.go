// non matching types in opassign
package test

func main() {
    type a []int
    var x1 a
    var x2 []int
    x1 += x2
}
