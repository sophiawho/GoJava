// Attempt to select a field that isn't defined
package main

type struct_type struct{
    x int
}

func main() {
    var myVar struct_type
    myVar.y = 1
}
