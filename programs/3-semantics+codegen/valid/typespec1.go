//~1
//~+1.000000e+000
//~97
//~a
//~true
//~2 2
//~+2.000000e+000 +2.000000e+000
//~98 98
//~b b
//~true true
//~3 3
//~+3.000000e+000 +3.000000e+000
//~99 99
//~c c
//~true true
//~

package main

type t_int int
type t_float64 float64
type t_rune rune
type t_string string
type t_bool bool

type t_array_int [3]int
type t_array_float64 [3]float64
type t_array_rune [3]rune
type t_array_string [3]string
type t_array_bool [3]bool

type t_slice_int []int
type t_slice_float64 []float64
type t_slice_rune []rune
type t_slice_string []string
type t_slice_bool []bool

type t_struct_base struct {
	x1 int
	x2 float64
	x3 rune
	x4 string
	x5 bool
	x6 [3]int
	x7 [3]float64
	x8 [3]rune
	x9 [3]string
	x10 [3]bool
	x11 []int
	x12 []float64
	x13 []rune
	x14 []string
	x15 []bool
}

type t_struct_new struct {
	x1 t_int
	x2 t_float64
	x3 t_rune
	x4 t_string
	x5 t_bool
	x6 t_array_int 
	x7 t_array_float64 
	x8 t_array_rune
	x9 t_array_string 
	x10 t_array_bool
	x11 t_slice_int 
	x12 t_slice_float64 
	x13 t_slice_rune 
	x14 t_slice_string 
	x15 t_slice_bool 
}

type t_struct_blankId struct {
	_ int
	x1, _, x2 int
	_, x3, _ int
	_, _, _ int
	x4, _, _ int
	_, _, x5 int
}

type t_struct_list struct {
	x1a, x1b int
	x2a, x2b float64
	x3a, x3b rune
	x4a, x4b string
	x5a, x5b bool
	x6a, x6b [3]int
	x7a, x7b [3]float64
	x8a, x8b [3]rune
	x9a, x9b [3]string
	x10a, x10b [3]bool
}

func main() {
	var x1a t_int = t_int(1)
	var x1b t_float64 = t_float64(1)
	var x1c t_rune = t_rune('a')
	var x1d t_string = t_string("a")
	var x1e t_bool	 = t_bool(true)

	println(x1a)
	println(x1b)
	println(x1c)
	println(x1d)
	println(x1e)

	var arr1 [3]int
	var arr2 [3]float64
	var arr3 [3]rune
	var arr4 [3]string
	var arr5 [3]bool

	arr1[0] = 2
	arr2[0] = 2.0
	arr3[0] = 'b'
	arr4[0] = "b"
	arr5[0] = true

	var x2a t_array_int
	var x2b t_array_float64
	var x2c t_array_rune
	var x2d t_array_string
	var x2e t_array_bool

	x2a[0] = 2
	x2b[0] = 2.0
	x2c[0] = 'b'
	x2d[0] = "b"
	x2e[0] = true

	println(arr1[0], x2a[0])
	println(arr2[0], x2b[0])
	println(arr3[0], x2c[0])
	println(arr4[0], x2d[0])
	println(arr5[0], x2e[0])

	var slice1 []int
	var slice2 []float64
	var slice3 []rune
	var slice4 []string
	var slice5 []bool

	slice1 = append(slice1, 3)
	slice2 = append(slice2, 3.0)
	slice3 = append(slice3, 'c')
	slice4 = append(slice4, "c")
	slice5 = append(slice5, true)

	var x3a t_slice_int
	var x3b t_slice_float64
	var x3c t_slice_rune
	var x3d t_slice_string
	var x3e t_slice_bool

	x3a = append(x3a, 3)
	x3b = append(x3b, 3.0)
	x3c = append(x3c, 'c')
	x3d = append(x3d, "c")
	x3e = append(x3e, true)

	println(slice1[0], x3a[0])
	println(slice2[0], x3b[0])
	println(slice3[0], x3c[0])
	println(slice4[0], x3d[0])
	println(slice5[0], x3e[0])

	var x4 t_struct_base
	x4.x1 = 4
	x4.x2 = 4.0
	x4.x3 = 'd'
	x4.x4 = "d"
	x4.x5 = true
	x4.x6 = arr1 // Null pointer
	x4.x7 = arr2
	x4.x8 = arr3
	x4.x9 = arr4
	x4.x10 = arr5

	var x5 t_struct_list
	x5.x1a = 5
	x5.x1b = 5
	x5.x2a = 5.0
	x5.x2b = 5.0
	x5.x3a = 'e'
	x5.x3b = 'e'
	x5.x4a = "e"
	x5.x4b = "e"
	x5.x5a = true
	x5.x5b = true
	x5.x6a = arr1
	x5.x6b = arr1
	x5.x7a = arr2
	x5.x7b = arr2
	x5.x8a = arr3
	x5.x8b = arr3
	x5.x9a = arr4
	x5.x9b = arr4
	x5.x10a = arr5
	x5.x10b = arr5

	var x6 t_struct_blankId
	x6.x1 = 6
	x6.x2 = 6
	x6.x3 = 6
	x6.x4 = 6
	x6.x5 = 6
}