#include "test_syntax.h"

// Note: NO EXPRESSION TESTING IN THIS FILE
/*************************************************************
 *
 *	PROLOG
 *
*************************************************************/


// PROLOG -> package main eol
TEST_F(DISABLED_SyntaxTest, prolog_incomplete) {
	// missing keyword main, EXPECT SYNTAX ERROR
	fprintf(stream, "package ");
	TESTVAL(ERROR_SYN);
}

// PROLOG -> package main eol
TEST_F(DISABLED_SyntaxTest, prolog_complete) {
	PROLOG;
	OPENFUN("main");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

/*************************************************************
 *
 *	FUNCTIONS
 *
*************************************************************/

// func main ()() {
TEST_F(DISABLED_SyntaxTest, function_incomplete) {
	PROLOG;
	fprintf(stream, "func main() ()\n");
	TESTVAL(ERROR_SYN);
}

// func main ()()
TEST_F(DISABLED_SyntaxTest, function_incomplete2) {
	PROLOG;
	OPENFUN("main");

	TESTVAL(ERROR_SYN);
}

// func ()() {\n}\n
TEST_F(DISABLED_SyntaxTest, function_incomplete3) {
	PROLOG;
	OPENFUN(" ");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main ()() {\n}\n
TEST_F(DISABLED_SyntaxTest, function_complete) {
	PROLOG;
	fprintf(stream, "func main ()() {\n}\n");

	TESTVAL(EXIT_SUCCESS);
}

// func main()(){\n}\n
// func foo()(){\n}\n
TEST_F(DISABLED_SyntaxTest, functions) {
	PROLOG;
	OPENFUN("main");
	CLOSEFUN;
	OPENFUN("foo");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func \n main () () {\n}
TEST_F(DISABLED_SyntaxTest, function_eol) {
	PROLOG;
	fprintf(stream, "func \n main () () {");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main\n () () {\n}
TEST_F(DISABLED_SyntaxTest, function_eol2) {
	PROLOG;
	fprintf(stream, "func main\n () () {");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main ()\n () {\n}
TEST_F(DISABLED_SyntaxTest, function_eol3) {
	PROLOG;
	fprintf(stream, "func main ()\n () {");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main () ()\n {\n}
TEST_F(DISABLED_SyntaxTest, function_eol4) {
	PROLOG;
	fprintf(stream, "func main () ()\n {");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main () () {}
TEST_F(DISABLED_SyntaxTest, function_eol5) {
	PROLOG;
	fprintf(stream, "func main () () {}");

	TESTVAL(ERROR_SYN);
}

/*************************************************************
 *
 *	FUNCTION PARAMETERS
 *
*************************************************************/

// func main ()() {\n}
TEST_F(DISABLED_SyntaxTest, params_empty) {
	PROLOG;
	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main (foo int)() {\n}
TEST_F(DISABLED_SyntaxTest, params) {
	PROLOG;
	fprintf(stream, "func fun (foo int) (){\n");
	CLOSEFUN;

	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main (foo int)() {\n}
TEST_F(DISABLED_SyntaxTest, params_inclomplete) {
	PROLOG;
	fprintf(stream, "func main (foo int,) (){\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main (foo float64, bar string) () {\n}
TEST_F(DISABLED_SyntaxTest, params2) {
	PROLOG;
	fprintf(stream, "func fun (foo float64, bar string) () {\n");
	CLOSEFUN;

	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main (foo float64,\n bar string) () {\n}
TEST_F(DISABLED_SyntaxTest, params_eol) {
	PROLOG;
	fprintf(stream, "func fun (foo float64,\n bar bool) () {\n");
	CLOSEFUN;

	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}


// func main (\nfoo float64,\n bar string) () {\n}
TEST_F(DISABLED_SyntaxTest, params_eol2) {
	PROLOG;
	fprintf(stream, "func main (\nfoo float64,\n bar bool) () {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main (\nfoo float64,\n bar string\n) () {\n}
TEST_F(DISABLED_SyntaxTest, params_eol3) {
	PROLOG;
	fprintf(stream, "func main (\nfoo float64,\n bar bool\n) () {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}


/*************************************************************
 *
 *	FUNCTION RETURN VALUES
 *
*************************************************************/

// func main () {\n}
TEST_F(DISABLED_SyntaxTest, return_val_empty) {
	PROLOG;
	fprintf(stream, "func main () {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main () () {\n}
TEST_F(DISABLED_SyntaxTest, return_val_empty2) {
	PROLOG;
	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main () (int) {\n}
TEST_F(DISABLED_SyntaxTest, return_val) {
	PROLOG;
	OPENFUN("main");
	CLOSEFUN;
	fprintf(stream, "func foo () (int) {\n return 1\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main () (int, ) {\n}
TEST_F(DISABLED_SyntaxTest, return_vals_incomplete) {
	PROLOG;
	fprintf(stream, "func main (int, ) {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main () (int, string, float64, bool) {\n}
TEST_F(DISABLED_SyntaxTest, return_vals) {
	PROLOG;
	fprintf(stream, "func foo () (int, string, float64, bool) {\n return 1, \"2\", 3.0, true\n");
	CLOSEFUN;

	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}


// func main () (\n int) {\n}
TEST_F(DISABLED_SyntaxTest, return_val_eol) {
	PROLOG;
	fprintf(stream, "func main() (\n int) {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main () (int, \n float64) {\n}
TEST_F(DISABLED_SyntaxTest, return_val_eol2) {
	PROLOG;
	fprintf(stream, "func foo () (int, \n float64) {\n return 1, 0.5\n");
	CLOSEFUN;

	OPENFUN("main");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// func main () (int, \n float64) {\n}
TEST_F(DISABLED_SyntaxTest, return_val_eol3) {
	PROLOG;
	fprintf(stream, "func main () (int\n, float) {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// func main () (int, \n float64 \n) {\n}
TEST_F(DISABLED_SyntaxTest, return_val_eol4) {
	PROLOG;
	fprintf(stream, "func main () (int, \n float \n) {\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}


/*************************************************************
 *
 *	VARIABLE DEFINITIONS
 *
*************************************************************/

// _ := 2
TEST_F(DISABLED_SyntaxTest, vardef_) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "_ := 2\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}

// i := 2
TEST_F(DISABLED_SyntaxTest, vardef_int) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := 2\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i := "Hello"
TEST_F(DISABLED_SyntaxTest, vardef_string) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := \"Hello world\"\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i := 1.234
TEST_F(DISABLED_SyntaxTest, vardef_float) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := 1.234\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i := true
TEST_F(DISABLED_SyntaxTest, vardef_true) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := true\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i := false

TEST_F(DISABLED_SyntaxTest, vardef_false) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := false\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i := j

TEST_F(DISABLED_SyntaxTest, vardef_od) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := j\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}


// := 2
TEST_F(DISABLED_SyntaxTest, vardef_no_id) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, " := 2\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}

// i 2
TEST_F(DISABLED_SyntaxTest, vardef_no_op) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i 2\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}

// i :=
TEST_F(DISABLED_SyntaxTest, vardef_no_expr) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i := \n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}


/*************************************************************
 *
 *	VARIABLE ASSIGNMENT
 *
*************************************************************/

// i /= 1
TEST_F(DISABLED_SyntaxTest, ass_div_int) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i /= 1\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i *= 1.234
TEST_F(DISABLED_SyntaxTest, ass_mul_float) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i *= 1.123\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}
// i -= j
TEST_F(DISABLED_SyntaxTest, ass_sub_id) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i -= j\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i += "world"
TEST_F(DISABLED_SyntaxTest, ass_add_string) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i += \" world\"\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i = true
TEST_F(DISABLED_SyntaxTest, ass_true) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i = true\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// i = false
TEST_F(DISABLED_SyntaxTest, ass_false) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i = false\n");
	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}


// = true
TEST_F(DISABLED_SyntaxTest, ass_no_id) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, " = true\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}

// i false
TEST_F(DISABLED_SyntaxTest, ass_no_op) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i false\n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}

// i +=
TEST_F(DISABLED_SyntaxTest, ass_no_exp) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "i += \n");
	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}


/*************************************************************
 *
 *	FUNCTION CALL
 *
*************************************************************/

// foo()
TEST_F(DISABLED_SyntaxTest, fun_call_no_args) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo()\n");
	CLOSEFUN;

	fprintf(stream, "func foo () {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// foo("Hello")
TEST_F(DISABLED_SyntaxTest, fun_call_arg) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo(\"Hello world\")\n");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 string) () {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// foo(2, "Hello", 1.123, true, bar)
TEST_F(DISABLED_SyntaxTest, fun_call_args) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo(2, \"Hello world\", 1.234, true, bar )\n");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 string, v3 float64, v4 bool, v5 int) () {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// foo(\n 2, true, bar)
TEST_F(DISABLED_SyntaxTest, fun_call_eol) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo(\n 2, true, bar )\n");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 bool, v3 int) {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// foo(2, \n true, \n bar)
TEST_F(DISABLED_SyntaxTest, fun_call_eol2) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo(2,\n true,\n bar)\n");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 bool, v3 int) {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// foo(2, \n true, \n bar \n)
TEST_F(DISABLED_SyntaxTest, fun_call_eol3) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo(\n 2,\n true,\n bar\n)\n");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 bool, v3 int) {\n");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}

// foo
TEST_F(DISABLED_SyntaxTest, fun_call_incomplete) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "foo");
	CLOSEFUN;

	TESTVAL(ERROR_SYN);
}


/*************************************************************
 *
 *	FUNCTION CALL & ASSIGN
 *
*************************************************************/

// i = foo("Hello")
TEST_F(DISABLED_SyntaxTest, ass_fun_call) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "i = foo(\"Hello world\")");
	CLOSEFUN;

	fprintf(stream, "func foo (bar string) {\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// i, j, k, l, m = foo(2, \"Hello world\", 1.234, true, bar)
TEST_F(DISABLED_SyntaxTest, ass_fun_call2) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "i, j, k, l, m = foo(2, \"Hello world\", 1.234, true, bar)");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 string, v3 float64, v4 bool, v5 int) (int,int,int,int,int) {\n return 1,2,3,4,5\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// i, j, k += foo(\n 2, true, bar)
TEST_F(DISABLED_SyntaxTest, ass_fun_call3) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "i, j, k = foo(\n 2, true, bar)");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 bool, v3 int) (int,int,int) {\n return 1,2,3\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}

// i, j, k += foo(2,\n true,\n bar)
TEST_F(DISABLED_SyntaxTest, ass_fun_call4) {
	PROLOG;
	OPENFUN("main");
	fprintf(stream, "i, j, k = foo(2,\n true,\n bar)");
	CLOSEFUN;

	fprintf(stream, "func foo (v1 int, v2 bool, v3 int) (int,int,int) {\n return 1,2,3\n");
	CLOSEFUN;

	TESTVAL(EXIT_SUCCESS);
}


/*************************************************************
 *
 *	CONDITIONALS
 *
*************************************************************/

// if foo == true {\n}
TEST_F(DISABLED_SyntaxTest, cond_if) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "if foo == true {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// if foo == true {\n}\
// else {\n}

TEST_F(DISABLED_SyntaxTest, cond_else) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "if foo == true {\n}");
	fprintf(stream, "else {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// if foo <= 1 {\n}\n
// else if foo >= 10 {\n}\n
TEST_F(DISABLED_SyntaxTest, cond_elseif) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "if foo <= 1 {\n}");
	fprintf(stream, "else if foo >= 10 {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// if foo <= 1 {\n}\n
// else if foo <= 10 {\n}\n
// else {\n}\n
TEST_F(DISABLED_SyntaxTest, cond_elseif_else) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "if foo <= 1 {\n} ");
	fprintf(stream, "else if foo <= 10 {\n\n} ");
	fprintf(stream, "else {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// if foo <= 1 {\n}\n
// else if foo <= 10 {\n}\n
// else if  foo >= 20 {\n}\n
// else {\n}\n

TEST_F(DISABLED_SyntaxTest, cond_complex) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "if foo <= 1 {\n}");
	fprintf(stream, "else if foo <= 10 {\n}");
	fprintf(stream, "else if foo >= 20 {\n}");
	fprintf(stream, "else {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// todo fail cases


/*************************************************************
 *
 *	ITERATIVE
 *
*************************************************************/

// for foo = 1; foo <= 10; foo += 1 {\n}\n
TEST_F(DISABLED_SyntaxTest, iterative) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "for foo := 1; foo <= 10; foo +=1 {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// for foo = 1; foo <= 10; {\n}\n
TEST_F(DISABLED_SyntaxTest, iterative2) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "for foo := 1; foo <= 10; {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// ; foo <= 10; foo += 1 {\n}\n
TEST_F(DISABLED_SyntaxTest, iterative3) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "for ; foo <= 10; foo +=1 {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// for ; foo <= 10; {\n}\n
TEST_F(DISABLED_SyntaxTest, iterative4) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "for ; foo <= 10; {\n}\n");

	CLOSEFUN;
	TESTVAL(EXIT_SUCCESS);
}

// for foo = 1; ; foo += 1 {\n}\n
TEST_F(DISABLED_SyntaxTest, iterative5) {
	PROLOG;
	OPENFUN("main");

	fprintf(stream, "for foo := 1; ; foo +=1 {\n}\n");

	CLOSEFUN;
	TESTVAL(ERROR_SYN);
}


// todo fail cases
