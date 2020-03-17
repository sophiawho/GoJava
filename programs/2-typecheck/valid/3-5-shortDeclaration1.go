package test

func main() {
    var a1 []int
    var a2 [3]int

    x1 := 1
    x2 := 1.0
    x3 := a1
    x4 := a2

    x5 := int(1)

    var a3 int = x1
    var a4 float64 = x2
    var a5 []int = x3
    var a6 [3]int = x4

    type num int
    var a7 num = num(3)
    x6_1 := num(2)
    x6, x7 := num(2), num(3)

    a7 = x6
    x6 = a7
}
