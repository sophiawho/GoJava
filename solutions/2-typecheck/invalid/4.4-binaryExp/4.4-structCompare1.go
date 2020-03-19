//invalid comparison of structs
package test

func bar() {
  var x struct { a int; b int; }
  var y struct { b int; a int; }
  print(x==y)
}
