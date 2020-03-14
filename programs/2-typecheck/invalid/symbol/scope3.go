// out of scope variable access

package test

func main() {
	for ;; {
		var a int = 3;
	}
	print(a);
}