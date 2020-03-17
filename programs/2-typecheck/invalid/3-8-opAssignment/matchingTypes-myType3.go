//non matching types in opassign
package test

func test() {
    var a [3]int
    var x1 a
    var x2 [3]int
    x1 += x2
}
