// non-void function containing infinite for loop with break but no return statement
package test

func bar() int {
  for {
    if true {
      print('X')
    } else {
      break
    }
    return 3
  }
}
