package test

type structType struct {
	a int
	b int
}

func main() {
	type num int
	var e, e2 int = structType.a, structType.a
	var e3 int = structType.x // this should be illegal
}
