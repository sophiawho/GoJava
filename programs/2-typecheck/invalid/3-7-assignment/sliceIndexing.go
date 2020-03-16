// Assign wrong type to slice indexed variable
package main

func f() {
    var a []int
    a[0] = "1"
}
