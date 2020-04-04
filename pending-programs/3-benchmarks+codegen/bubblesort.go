// Bubble Sort in Golang
package main
 
import (
    "fmt"
    "time"
)
 
func main() {
   	start := time.Now()
	
	var slice []int
	var n int = 99999999
	for i := 0; i<70000; i++ {
		slice = append(slice, n)
		n--
	}

    bubblesort(slice)

	duration := time.Since(start)	
	fmt.Println(duration)
    fmt.Println("done sort")
}
  
func bubblesort(items []int) {
    var n = len(items)
    var sorted = false
    for !sorted {
        swapped := false
        for i := 0; i < n-1; i++ {
            if items[i] > items[i+1] {
                items[i+1], items[i] = items[i], items[i+1]
                swapped = true
            }
        }
        if !swapped {
            sorted = true
        }
        n = n - 1
    }
}