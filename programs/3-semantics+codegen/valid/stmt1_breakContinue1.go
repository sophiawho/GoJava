//~0
//~1
//~0
//~1
//~1
//~0
//~1
//~2
//~0
//~1
//~2
//~1
//~2
//~2
//~0
//~Continued
//~Broke

package main

func main() {
    var x1 int = 0
    for ; x1 < 1; x1++ {
        println(x1)
        continue
    }
    println(x1)

    var x2 int = 0
    for ; ; x2++ {
        println(x2)
        if (x2 < 1) {
            continue
        } else {
            break
        }
    }
    // x2 doesn't increment after a break
    println(x2)

    for x3 := 0; x3 < 3; x3++ {

        for x4 := x3; x4 < 3; x4++ {
            println(x4)

            for ; ; {
                if x4 >= 0 {
                    break
                }
            }
            continue
        }

        println(x3)
        continue
    }

    for a:=0; a < 1; a++ {
        println(a)
        continue
    }
    println("Continued")

    for ; ; {
        break
    }
    print("Broke")

}
