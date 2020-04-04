//~done sort
//~

// Bubble Sort in Golang
package main

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
 
func main() {
	var slice []int
 var n int = 99999999
 for i := 0; i<70000; i++ {
	 slice = append(slice, n)
	 n--
 }

 bubblesort(slice)

 println("done sort")
}