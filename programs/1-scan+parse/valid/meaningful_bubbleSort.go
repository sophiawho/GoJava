// create and traverse a binary tree implemented with an array

package main

func main() {
	var list []int 
	list = append(list, 10)
	list = append(list, 7)
	list = append(list, 99)
	list = append(list, 1)
	list = bubble_sort(list)
	
	println(list)
}

func bubble_sort(list []int) []int {
	var swap bool

	for swap {
		swap = false
		for i := 0; i < (len(list) - 1); i++ {
			if list[i] > list[i + 1] {
				tmp := list[i]
				list[i] = list[i + 1]
				list[i + 1] = tmp
				swap = true
			}
		}
	}
	return list
}