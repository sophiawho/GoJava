//valid non-void function declarations with infinite for loops
package test

func yolo() int {
  for {
    for {
      break
    }
  }
}

func swag() int {
  for {
    for {
      if true {
        break
      }
    }
  }
}

func swag2() int {
  var a = 3;
  for ;; a += 3 {

  }
}

func swag3() {
  for {
  }
}