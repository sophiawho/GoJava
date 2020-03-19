//invalid comparison of structs
package test

func bar() {
  var x struct { a int; }
  var y struct { b int; }
  print(x==y)
}
