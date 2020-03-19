package test

type myType1 int
type myType2 float64
type myType3 []int
type myType4 [3]int
type myType5 struct {
    field1 int
    field2 float64
    field3 []int
    field4 [3]int
    field5, field6 myType1
}

func main() {
    var x1 myType1
    var x2 myType2
    
    var x3 myType3
    var x4 myType4
    var x5 myType5

    var a1 int
    var a2 float64

    a1 += 1
    a1 -= 1
    a1 *= 1
    a1 /= 1
    a1 %= 1
    a1 &= 1
    a1 |= 1
    a1 ^= 1

    a2 += 1.0
    a2 -= 1.0
    a2 *= 1.0
    a2 /= 1.0

    x5.field1 += a1
    x5.field1 -= a1
    x5.field1 *= a1
    x5.field1 /= a1
    x5.field1 %= a1
    x5.field1 &= a1
    x5.field1 |= a1
    x5.field1 ^= a1

    x5.field2 += a2
    x5.field2 -= a2
    x5.field2 *= a2
    x5.field2 /= a2

}
