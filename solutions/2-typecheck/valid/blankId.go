// valid blank identifier
package test

func foo() int {
	return 0
}

var _ = foo()

var _ int

type _ int

type _ struct {
	_ int
}

func _(_ int) {	
	_, b := 0,0
	_ = 0
}

func _(_ int, _ float64) int {
	return 0
}

func _() float64 {
	return 0.0
}

func main() {
	var _ int
	var _ float64

	type _ int
	type _ struct {
		_ int
		_ float64
	}

	_, c := 0, 0
}