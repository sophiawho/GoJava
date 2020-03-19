//Invalid because of the break in the if statement
package test

func test() int {
	for {
		if true {
		} else {
			break
		}
	}
}
