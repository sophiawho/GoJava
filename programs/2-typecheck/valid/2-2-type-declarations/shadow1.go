// If T1 is already declared in the current scope, an error is raised. If T1 is
// already declared but in an outer scope, the new T1 -> def(T2) type mapping
// will shadow the previous mapping
package test

func f() {
    type a int
    {
        type a float64
    }
}
