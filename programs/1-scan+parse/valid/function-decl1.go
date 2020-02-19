package main

// Without params

//// With returns
func f() t_type {return a;}

func f() []t_type {return a;}

func f() [3]t_type {return a;}

//// Without returns
func f() {}


// With params

//// With returns
////// short form
func f(a, b int, c string, d, e t_type) t_type {return f;}

func f(a, b int, c string, d, e t_type) []t_type {return f;}

func f(a, b int, c string, d, e t_type) [3]t_type {return f;}

func f(a, b []t_type) t_type {return f;}

func f(a, b []t_type) []t_type {return f;}

func f(a, b []t_type) [3]t_type {return f;}

func f(a, b [3]t_type) t_type {return f;}

func f(a, b [3]t_type) []t_type {return f;}

func f(a, b [3]t_type) [3]t_type {return f;}

////// long form
func f(b int, c string, e t_type) t_type {return f;}

func f(b int, c string, e t_type) []t_type {return f;}

func f(b int, c string, e t_type) [3]t_type {return f;}

func f(b []t_type) t_type {return f;}

func f(b []t_type) []t_type {return f;}

func f(b []t_type) [3]t_type {return f;}

func f(b [3]t_type) t_type {return f;}

func f(b [3]t_type) []t_type {return f;}

func f(b [3]t_type) [3]t_type {return f;}

//// Without returns
////// short form
func f(a, b int, c string, d, e t_type) {}

func f(a, b int, c string, d, e t_type) {}

func f(a, b int, c string, d, e t_type) {}

func f(a, b []t_type) {}

func f(a, b []t_type) {}

func f(a, b []t_type) {}

func f(a, b [3]t_type) {}

func f(a, b [3]t_type) {}

func f(a, b [3]t_type) {}

////// long form
func f(b int, c string, e t_type) {}

func f(b int, c string, e t_type) {}

func f(b int, c string, e t_type) {}

func f(b []t_type) {}

func f(b []t_type) {}

func f(b []t_type) {}

func f(b [3]t_type) {}

func f(b [3]t_type) {}

func f(b [3]t_type) {}


