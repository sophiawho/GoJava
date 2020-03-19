// valid comparisons of variables of different types
package test

func foo() {
  var a [3]int
  var b [3]int
  print(a==b)

  type point struct { x, y float64; }
  var s point
  var t point
  print(s==t)

  type ar [7]bool
  var u ar
  var v ar
  print(u==v)

  var c string
  var d string
  print(c<d)

  var m struct { bar int; }
  var n struct { bar int; }
  print(m==n)
}
