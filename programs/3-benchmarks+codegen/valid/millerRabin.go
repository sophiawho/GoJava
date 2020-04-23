//~5 complete
//~7 complete
//~9 complete
//~11 complete
//~13 complete
//~15 complete
//~17 complete
//~19 complete
//~21 complete
//~23 complete
//~25 complete
//~27 complete
//~29 complete
//~31 complete
//~33 complete
//~35 complete
//~37 complete
//~39 complete
//~41 complete
//~43 complete
//~45 complete
//~47 complete
//~49 complete
//~51 complete
//~53 complete
//~55 complete
//~57 complete
//~59 complete
//~61 complete
//~63 complete
//~65 complete
//~67 complete
//~69 complete
//~71 complete
//~73 complete
//~75 complete
//~77 complete
//~79 complete
//~81 complete
//~83 complete
//~85 complete
//~87 complete
//~89 complete
//~91 complete
//~93 complete
//~95 complete
//~97 complete
//~99 complete
//~101 complete
//~103 complete
//~105 complete
//~107 complete
//~109 complete
//~111 complete
//~113 complete
//~115 complete
//~117 complete
//~119 complete
//~121 complete
//~123 complete
//~125 complete
//~127 complete
//~129 complete
//~131 complete
//~133 complete
//~135 complete
//~137 complete
//~139 complete
//~141 complete
//~143 complete
//~145 complete
//~147 complete
//~149 complete
//~151 complete
//~153 complete
//~155 complete
//~157 complete
//~159 complete
//~161 complete
//~163 complete
//~165 complete
//~167 complete
//~169 complete
//~171 complete
//~173 complete
//~175 complete
//~177 complete
//~179 complete
//~181 complete
//~183 complete
//~185 complete
//~187 complete
//~189 complete
//~191 complete
//~193 complete
//~195 complete
//~197 complete
//~199 complete
//~201 complete
//~203 complete
//~205 complete
//~207 complete
//~209 complete
//~211 complete
//~213 complete
//~215 complete
//~217 complete
//~219 complete
//~221 complete
//~223 complete
//~225 complete
//~227 complete
//~229 complete
//~231 complete
//~233 complete
//~235 complete
//~237 complete
//~239 complete
//~241 complete
//~243 complete
//~245 complete
//~247 complete
//~249 complete

package main

func power(base, exp int) int {
	var result = base
	for i := 0; i < exp-1; i++ {
		result *= base
	}
	return result
}

var a, c, seed int = 242685, 7, 42
var low int = 2

type A struct {
	d int
	r int
}

func factor(n int) A {
	var d = 1
	var r = 0
	var result A
	result.d = d
	result.r = r

	var found = false

	for d = 0; d < 50; d = d + 2 {
		for r = 0; r < 50; r++ {
			x := n - (power(2, r)*d + 1)
			if x == 0 {
				found = true
			}
			if found {
				break
			}
		}
		if found {
			break
		}
	}

	if !found {
		for r = 0; r < 50; r++ {
			for d = 1; d < 50; d = d + 2 {
				x := n - (power(2, r)*d + 1)
				if x == 0 {
					found = true
				}
				if found {
					break
				}
			}
			if found {
				break
			}
		}
	}

	result.d = d
	result.r = r
	return result
}

func millerRabin(n int, rounds int) string {
	var fac A
	fac = factor(n)
	d, r := fac.d, fac.r
	for i := 0; i < rounds; i++ {
		// For n < 2,047: a = 2
		a := 2
		x := power(a, d) % n
		if x == 1 || x == n-1 {
			continue
		}
		for j := 0; j < r-1; j++ {
			x = power(x, 2) % n
			if x == n-1 {
				continue
			}
		}
		if true {
			return "composite"
		}
	}
	return "probably prime"
}

func main() {
	for i := 5; i < 250; i = i + 2 {
		a := millerRabin(i, 20)
		println(i, "complete")
	}
}
