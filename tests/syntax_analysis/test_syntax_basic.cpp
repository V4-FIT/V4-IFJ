#include "test_syntax.h"

//Program  -> Prolog Functions eof

// Prolog ->  package main eol
TEST_F(SyntaxTest, prolog) {
	// missing keyword main, EXPECT SYNTAX ERROR
	fprintf(stream, "package ");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);

	fprintf(stream, "main\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
}

// Functions -> Function Functions
TEST_F(SyntaxTest, functions) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func main()() {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func ");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);

	fprintf(stream, "bar()()");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);

	fprintf(stream, " {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "for foo() () {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);

}
 
// Param_list -> Param Param_n
// Param_list -> eps
// Param_n -> , Param Param_n
// Param_n -> eps
// Param ->  id Typename
TEST_F(SyntaxTest, params) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo() {\n}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func main(int foo) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func bar(float64 foo,\n string bar,\n bool var) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// missing param_n, EXPECT SYNTAX ERROR
	fprintf(stream, "func foo(int foo, ) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

TEST_F(SyntaxTest, params2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo(int foo, if bar) {\n}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// Return_list -> ( Returns )
// Return_list -> eps
// Returns ->  Typename Type_n
// Returns -> eps
// Type_n ->  , Typename Type_n
// Type_n -> eps
TEST_F(SyntaxTest, return_list) {
	fprintf(stream, "package main\nfunc main() {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func bar()() {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func foo()(int) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func foobar()(int,\n string,\n float64,\n bool) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// missing type_n, EXPECT SYNTAX ERROR
	fprintf(stream, "func barfoo()(int, string,) {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// wrong keyword, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, return_list2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "funct main()(int, boo) {\n}");
	rewind(stream);

	EXPECT_EQ(parse(stream), ERROR_SYN);
}


// Statements -> Statement Statements
// Statements -> eps
// Statement ->  Var_define -> id := Expression eol
// Statement -> Return eol
TEST_F(SyntaxTest, statements) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo() () {\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// RETURN
	fprintf(stream, "func bar() (int) {\n");
	fprintf(stream, "return 2\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// VAR DEF (int)
	// RETURN
	fprintf(stream, "func foobar() (int) {\n");
	fprintf(stream, "foo := 2");
	fprintf(stream, "return foo\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// VAR DEF (string, float, bool)
	// RETURN
	fprintf(stream, "func barfoo() (string) {\n");
	fprintf(stream, "foo := \"Hello World!\"");
	fprintf(stream, "bar := 1.123");
	fprintf(stream, "fb := -121.123");
	fprintf(stream, "bf := true");
	fprintf(stream, "return foo\n}\n");
	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// empty return, EXPECT SYNTAX ERROR
	fprintf(stream, "funct foo()() {\n");
	fprintf(stream, "return\n");
	fprintf(stream, "}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), ERROR_SYN);

}

// missing expression, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, statements2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo() () {\n");
	fprintf(stream, "var := \n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}


// Var_define -> id := Expression eol
// Def_Ass_Call2 -> , Ids AssignOp Exprs_FunCall
TEST_F(SyntaxTest, define_assign) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo()() {\n");

	fprintf(stream, "foobar := true\n");
	fprintf(stream, "foobar = false\n");

	fprintf(stream, "foo := 1\n");
	fprintf(stream, "foo -= 1\n");

	fprintf(stream, "bar := \"Hello\"\n");
	fprintf(stream, "bar += \" world!\"\n");

	fprintf(stream, "barfoo := 1.0\n");
	fprintf(stream, "barfoo /= -10\n");

	// ids assignOp expressions
	fprintf(stream, "bar, barfoo *= 1, 10\n");

	fprintf(stream, "\n}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// missing right side, EXPECT SYNTAX ERROR

	fprintf(stream, "func bar()() {\n");
	fprintf(stream, "foo, bar = \n");
	rewind(stream);

	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// wrong operand, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, define_assign2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func main()() {\n");
	fprintf(stream, "foo, bar := 1, 2n\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}


// Statement -> Conditionals eol
TEST_F(SyntaxTest, conditionals) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo()() {\n");

	// Conditionals -> Conditional Conditional_n
	// Conditional -> if Expression { eol Statements }
	// Conditional_n -> eps
	fprintf(stream, "if true {\n}\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);	

	// Conditional -> if Expression { eol Statements }
	// Conditional_n ->  else Else
	// Else ->  Conditional Conditional_n
	// Else -> { eol Statements }
	fprintf(stream, "func bar(int boo, bool foo)(int) {\n");
	fprintf(stream, "if boo == 1 {\n");
	fprintf(stream, "\tboo = 100\n");

	fprintf(stream, "} else if boo == 2 {\n");
	fprintf(stream, "\tboo = 200\n");

	fprintf(stream, "} else {\n");
	fprintf(stream, "\tboo = 300\n");
	fprintf(stream, "}\n\n");

	fprintf(stream, "if foo == true {\n");
	fprintf(stream, "\treturn boo\n");

	fprintf(stream, "} else {\n");
	fprintf(stream, "\treturn 0-boo\n}\n}");

	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// missing expression, EXPECT SYNTAX ERROR

	fprintf(stream, "func foo()() {\n");
	fprintf(stream, "if {\n}\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// missing brackets, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, conditionals2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo()() {\n");
	fprintf(stream, "if boo == true boo = false\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// Statement -> Iterative
// Iterative -> for Var_define ; Expression ; Assignment { Statements }
TEST_F(SyntaxTest, iterative) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo(int bar)(){\n");

	// var_define -> eps
	// assignment -> eps
	fprintf(stream, "for ; bar <= 5 ; {\n");
	fprintf(stream, "bar += 1\n}\n");
	fprintf(stream, "}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func bar()(){\n");

	// assignment -> eps
	fprintf(stream, "for bar := 0 ; bar < 5 ; {\n");
	fprintf(stream, "bar += 1\n}\n");
	fprintf(stream, "}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
	fprintf(stream, "func bar()(){\n");

	// full iterative
	fprintf(stream, "for bar := 0; bar != 10; bar += 1 {\n");
	fprintf(stream, "bar += 1\n}\n");
	fprintf(stream, "}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);
	
	// missing expression, EXPECT SYNTAX ERROR

	fprintf(stream, "func main()() {\n");
	fprintf(stream, "for ; ; {\n}\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// missing brackets, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, iterative2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func main()(){\n");
	fprintf(stream, "for foo := 0 ; foo != 10 ; foo += 1\n");
	fprintf(stream, "}");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// FunctionCall -> Id ( Eol_opt Arguments )
TEST_F(SyntaxTest, functionCalls) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func foo()(int, float64) {\n");

	fprintf(stream, "print(\"Hello World\\n\")\n");
	fprintf(stream, "return 1, 1.1\n");
	fprintf(stream, "}\n");
	rewind(stream);

	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	fprintf(stream, "func bar(int foo, int bar)() {\n");
	fprintf(stream, "foo, bar = foo()\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), EXIT_SUCCESS);

	// missing bracket, EXPECT SYNTAX ERROR

	fprintf(stream, "func main()() {\n");
	fprintf(stream, "\tfoo(\n");
	fprintf(stream, "}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// missing argument, EXPECT SYNTAX ERROR
TEST_F(SyntaxTest, functionCalls2) {
	fprintf(stream, "package main\n");
	fprintf(stream, "func main()(){\n");
	fprintf(stream, "foo(bar, )\n");
	fprintf(stream, "}\n");
	fprintf(stream, "foo()(){\n}\n");

	rewind(stream);
	EXPECT_EQ(parse(stream), ERROR_SYN);
}

// todo test EOLS
