// attempt to op assign incompatible types
package test

func f() {
    var a int = 1
    a += 1.0
}
