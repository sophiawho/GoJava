// linked list traversal

package main

type node struct {
	data int;
	next int;
	free bool;
}

var list [100]node
var list_head = -1

func main() {
	add_item(10)
	add_item(23)
	add_item(-5000)
	print_list()
}

func init() {
	for i := 0; i < cap(list); i++ {
		list[i].free = true
		list[i].next = -1
	}
}

func get_next_free_index() int {
	for i := 0; i < cap(list); i++ {
		if (list[i].free) {
			return i
		} 
	}
	return -1
}

func add_item(item int) {
	if (list_head == -1) {
		list[0].data = item
	} else {
		var last_index int
		var i = list_head
		for list[i].next != -1 {
			i = list[i].next
		}
		next_index = get_next_free_index()
		list[last_index] = next_index
		list[next_index] = item
	}
}

func print_list() {
	for i := list_head; (i != -1); i = list[i].next {
		println(list[i])
	}
}