// cannot declare functions in structs
package main

type p struct {
    func f1() {}
}

