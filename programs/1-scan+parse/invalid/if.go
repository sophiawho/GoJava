// unexpected else at end of statement
package main

func main() {
	if 1 {
		print("sfsfdfd")
	} else {
		var x = 1
	} else if true {
		var x = 2
	}
}
