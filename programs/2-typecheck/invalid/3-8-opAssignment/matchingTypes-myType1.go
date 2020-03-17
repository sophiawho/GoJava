// non matching types in opassign
package test

func test() {
    type a int
    var x1 int
    var x2 a
    x1 += x2
}
