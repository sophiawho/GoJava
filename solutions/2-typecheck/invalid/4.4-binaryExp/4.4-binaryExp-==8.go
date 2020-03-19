//invalid struct comparisons
package test

func foo() {
  type list struct { l []int; }
  var a list
  var b list
  print(a==b)
}
