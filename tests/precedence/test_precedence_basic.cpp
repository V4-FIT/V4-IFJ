#include "test_precedence.h"

/***************************************
*
*   BRACKETS
*
***************************************/

// ()
TEST_F(Precedence, brackets_empty) {
	fprintf(stream, "()");

	TESTVAL(ERROR_SYN);
}

// (a)
TEST_F(Precedence, brackets_id) {
	fprintf(stream, "(a)");

	TESTVAL(EXIT_SUCCESS);
}

// ((10))
TEST_F(Precedence, brackets_int) {
	fprintf(stream, "((10))");

	TESTVAL(EXIT_SUCCESS);
}

// (((12.3)))
TEST_F(Precedence, brackets_float) {
	fprintf(stream, "(((12.3)))");

	TESTVAL(EXIT_SUCCESS);
}

// ((("Hello")))
TEST_F(Precedence, brackets_string) {
	fprintf(stream, "((((\"Hello\"))))");

	TESTVAL(EXIT_SUCCESS);
}

// ((((true))))
TEST_F(Precedence, brackets_bool) {
	fprintf(stream, "((((true))))");

	TESTVAL(EXIT_SUCCESS);
}


// (a
TEST_F(Precedence, brackets_rigt_missing) {
	fprintf(stream, "(a");

	TESTVAL(ERROR_SYN);
}

// a)
TEST_F(Precedence, brackets_left_missing) {
	fprintf(stream, "a)");

	TESTVAL(ERROR_SYN);
}

// (a)(a)
TEST_F(Precedence, brackets_missing_op) {
	fprintf(stream, "(a)(a)");

	TESTVAL(ERROR_SYN);
}


/***************************************
*
*   UNARY OPERATORS (+, -, !)
*
***************************************/

// !a
TEST_F(Precedence, un_neg_id) {
	fprintf(stream, "!a");

	TESTVAL(EXIT_SUCCESS);
}

// !true
TEST_F(Precedence, un_neg_true) {
	fprintf(stream, "!true");

	TESTVAL(EXIT_SUCCESS);
}

// !false
TEST_F(Precedence, un_neg_false) {
	fprintf(stream, "!false");

	TESTVAL(EXIT_SUCCESS);
}

// !!!a
TEST_F(Precedence, un_neg_id_multi) {
	fprintf(stream, "!!!a");

	TESTVAL(EXIT_SUCCESS);
}

// !(!a)
TEST_F(Precedence, un_neg_id_multi_brackets) {
	fprintf(stream, "!(!a)");

	TESTVAL(EXIT_SUCCESS);
}

// +a
TEST_F(Precedence, un_plus_id) {
	fprintf(stream, "+a");

	TESTVAL(EXIT_SUCCESS);
}

// -a
TEST_F(Precedence, un_minus_id) {
	fprintf(stream, "-a");

	TESTVAL(EXIT_SUCCESS);
}

// +10
TEST_F(Precedence, un_plus_int) {
	fprintf(stream, "+10");

	TESTVAL(EXIT_SUCCESS);
}

// -1O
TEST_F(Precedence, un_minus_int) {
	fprintf(stream, "-10");

	TESTVAL(EXIT_SUCCESS);
}

// ++--+-10
TEST_F(Precedence, un_plus_minus_combo1) {
	fprintf(stream, "++--+-10");

	TESTVAL(EXIT_SUCCESS);
}

// --+-++-10.12
TEST_F(Precedence, un_plus_minus_combo2) {
	fprintf(stream, "--+-++-10.12");

	TESTVAL(EXIT_SUCCESS);
}

// -(-(+((a))))
TEST_F(Precedence, un_plus_minus_combo3) {
	fprintf(stream, "--+-++-10.12");

	TESTVAL(EXIT_SUCCESS);
}

// +
TEST_F(Precedence, un_plus_missing_term) {
	fprintf(stream, "+");

	TESTVAL(ERROR_SYN);
}

// -
TEST_F(Precedence, un_minus_missing_term) {
	fprintf(stream, "-");

	TESTVAL(ERROR_SYN);
}

// +
TEST_F(Precedence, un_neg_missing_term) {
	fprintf(stream, "!");

	TESTVAL(ERROR_SYN);
}

/***************************************
*
*   BINARY OPERATORS 1 ( *, / )
*
***************************************/

// a*b
TEST_F(Precedence, mul_id) {
	fprintf(stream, "a*b");

	TESTVAL(EXIT_SUCCESS);
}

// 10*10
TEST_F(Precedence, mul_int) {
	fprintf(stream, "10*10");

	TESTVAL(EXIT_SUCCESS);
}

// 10*a
TEST_F(Precedence, mul_id_int) {
	fprintf(stream, "10*a");

	TESTVAL(EXIT_SUCCESS);
}

// a*10.1
TEST_F(Precedence, mul_id_float) {
	fprintf(stream, "a*10.1");

	TESTVAL(EXIT_SUCCESS);
}

// 12.1*23.23
TEST_F(Precedence, mul_float) {
	fprintf(stream, "12.1*2.23");

	TESTVAL(EXIT_SUCCESS);
}

// a/b
TEST_F(Precedence, div_id) {
	fprintf(stream, "a*b");

	TESTVAL(EXIT_SUCCESS);
}

// 10/10
TEST_F(Precedence, div_int) {
	fprintf(stream, "10/10");

	TESTVAL(EXIT_SUCCESS);
}

// 10/a
TEST_F(Precedence, div_int_id) {
	fprintf(stream, "10/a");

	TESTVAL(EXIT_SUCCESS);
}

// 12.1/10.12
TEST_F(Precedence, div_float) {
	fprintf(stream, "12.1/10.12");

	TESTVAL(EXIT_SUCCESS);
}

// 12.1/a
TEST_F(Precedence, div_float_id) {
	fprintf(stream, "12.1/a");

	TESTVAL(EXIT_SUCCESS);
}

// a*b*c
TEST_F(Precedence, mul_id_multi) {
	fprintf(stream, "a*b*c");

	TESTVAL(EXIT_SUCCESS);
}

// a/b/c
TEST_F(Precedence, div_id_multi) {
	fprintf(stream, "a/b/c");

	TESTVAL(EXIT_SUCCESS);
}

// (10.0*a)/(12*b)
TEST_F(Precedence, div_mul_combo1) {
	fprintf(stream, "(10.0*a)/(+12*b)");

	TESTVAL(EXIT_SUCCESS);
}

// 10.0*(a/12)*b
TEST_F(Precedence, div_mul_combo21) {
	fprintf(stream, "10.0*(a/-12)*b");

	TESTVAL(EXIT_SUCCESS);
}

// 10.0*(a/(12*b))
TEST_F(Precedence, div_mul_combo3) {
	fprintf(stream, "10.0*(a/(12*b))");

	TESTVAL(EXIT_SUCCESS);
}

// a*
TEST_F(Precedence, mul_missing_term) {
	fprintf(stream, "a*");

	TESTVAL(ERROR_SYN);
}

// b/
TEST_F(Precedence, div_missing_term) {
	fprintf(stream, "b/");

	TESTVAL(ERROR_SYN);
}

// /a
TEST_F(Precedence, div_missing_term2) {
	fprintf(stream, "/a");

	TESTVAL(ERROR_SYN);
}

// *b
TEST_F(Precedence, mul_missing_term2) {
	fprintf(stream, "*b");

	TESTVAL(ERROR_SYN);
}

// /***************************************
// *
// *   BINARY OPERATORS 2 ( +, - )
// *
// ***************************************/

// a+a
TEST_F(Precedence, add_id) {
	fprintf(stream, "a+a");

	TESTVAL(EXIT_SUCCESS);
}

// a+1
TEST_F(Precedence, add_id_int) {
	fprintf(stream, "a+1");

	TESTVAL(EXIT_SUCCESS);
}

// a+12.1
TEST_F(Precedence, add_id_float) {
	fprintf(stream, "a+12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1+12
TEST_F(Precedence, add_int) {
	fprintf(stream, "1+12");

	TESTVAL(EXIT_SUCCESS);
}

// 12.12+12.12
TEST_F(Precedence, add_float) {
	fprintf(stream, "12.12+12.12");

	TESTVAL(EXIT_SUCCESS);
}

// a-b
TEST_F(Precedence, sub_id) {
	fprintf(stream, "a-b");

	TESTVAL(EXIT_SUCCESS);
}

// a-1
TEST_F(Precedence, sub_id_int) {
	fprintf(stream, "a-1");

	TESTVAL(EXIT_SUCCESS);
}

// a-12.1
TEST_F(Precedence, sub_id_float) {
	fprintf(stream, "a-12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1-12
TEST_F(Precedence, sub_int) {
	fprintf(stream, "1-12");

	TESTVAL(EXIT_SUCCESS);
}

// 12.12-23.1
TEST_F(Precedence, sub_float) {
	fprintf(stream, "12.12-23.1");

	TESTVAL(EXIT_SUCCESS);
}


// a+112-(b+12)
TEST_F(Precedence, add_sub_combo1) {
	fprintf(stream, "a+112-(b+12)");

	TESTVAL(EXIT_SUCCESS);
}

// (-(a-10)-(b+12))+(-12-a)
TEST_F(Precedence, add_sub_combo2) {
	fprintf(stream, " (-(a-10)-(b+12))+(-12-a)");

	TESTVAL(EXIT_SUCCESS);
}

// (a+b)/(b-a)-12
TEST_F(Precedence, add_sub_combo3) {
	fprintf(stream, "(a+b)/(b-a)-12");

	TESTVAL(EXIT_SUCCESS);
}

// a+(b/(b-a)-12)
TEST_F(Precedence, add_sub_combo4) {
	fprintf(stream, "a+(b/(b-a)-12)");

	TESTVAL(EXIT_SUCCESS);
}

// a+
TEST_F(Precedence, add_missing_term) {
	fprintf(stream, "a+");

	TESTVAL(ERROR_SYN);
}

// a-
TEST_F(Precedence, sub_missing_term) {
	fprintf(stream, "a-");

	TESTVAL(ERROR_SYN);
}


/***************************************
*
*   COMPARATORS 1 ( <, <=, >, >= )
*
***************************************/

// a<b
TEST_F(Precedence, lt_id) {
	fprintf(stream, "a<b");

	TESTVAL(EXIT_SUCCESS);
}

// a<1
TEST_F(Precedence, lt_id_int) {
	fprintf(stream, "a<1");

	TESTVAL(EXIT_SUCCESS);
}

// a<12.1
TEST_F(Precedence, lt_id_float) {
	fprintf(stream, "a<12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1<12
TEST_F(Precedence, lt_int) {
	fprintf(stream, "1<12");

	TESTVAL(EXIT_SUCCESS);
}

// 1.23<1.1
TEST_F(Precedence, lt_float) {
	fprintf(stream, "1.23<1.1");

	TESTVAL(EXIT_SUCCESS);
}


// a<=b
TEST_F(Precedence, le_id) {
	fprintf(stream, "a<=b");

	TESTVAL(EXIT_SUCCESS);
}

// a<=1
TEST_F(Precedence, le_id_int) {
	fprintf(stream, "a<=1");

	TESTVAL(EXIT_SUCCESS);
}

// a<=12.1
TEST_F(Precedence, le_id_float) {
	fprintf(stream, "a<=12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1<=12
TEST_F(Precedence, le_int) {
	fprintf(stream, "1<=12");

	TESTVAL(EXIT_SUCCESS);
}

// 1.23<=1.1
TEST_F(Precedence, le_float) {
	fprintf(stream, "1.23<=1.1");

	TESTVAL(EXIT_SUCCESS);
}


// a>b
TEST_F(Precedence, gt_id) {
	fprintf(stream, "a>b");

	TESTVAL(EXIT_SUCCESS);
}

// a>1
TEST_F(Precedence, gt_id_int) {
	fprintf(stream, "a>1");

	TESTVAL(EXIT_SUCCESS);
}

// a>12.1
TEST_F(Precedence, gt_id_float) {
	fprintf(stream, "a>12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1>12
TEST_F(Precedence, gt_int) {
	fprintf(stream, "1>12");

	TESTVAL(EXIT_SUCCESS);
}

// 1.23>1.1
TEST_F(Precedence, gt_float) {
	fprintf(stream, "1.23>1.1");

	TESTVAL(EXIT_SUCCESS);
}


// a>=b
TEST_F(Precedence, ge_id) {
	fprintf(stream, "a>=b");

	TESTVAL(EXIT_SUCCESS);
}

// a>=1
TEST_F(Precedence, ge_id_int) {
	fprintf(stream, "a>=1");

	TESTVAL(EXIT_SUCCESS);
}

// a>=12.1
TEST_F(Precedence, ge_id_float) {
	fprintf(stream, "a>=12.1");

	TESTVAL(EXIT_SUCCESS);
}

// 1>=12
TEST_F(Precedence, ge_int) {
	fprintf(stream, "1>=12");

	TESTVAL(EXIT_SUCCESS);
}

// 1.23>=1.1
TEST_F(Precedence, ge_float) {
	fprintf(stream, "1.23>=1.1");

	TESTVAL(EXIT_SUCCESS);
}


// -(12+12*b/(b*23)-a) < 12*(123-a+b)
TEST_F(Precedence, lt_combo) {
	fprintf(stream, "-(12+12*b/(b*23)-a) < 12*(123-a+b)");

	TESTVAL(EXIT_SUCCESS);
}

// -(12+12*b)/(b*23-a) <= 12*123-a+b
TEST_F(Precedence, le_combo) {
	fprintf(stream, "-(12+12*b)/(b*23-a) <= 12*123-a+b");

	TESTVAL(EXIT_SUCCESS);
}

// -12+(12*b/b)*23-a) > 12*(123-a)+b)
TEST_F(Precedence, gt_combo) {
	fprintf(stream, "(-12+(12*b/b)*23-a > 12*(123-a)+b)");

	TESTVAL(EXIT_SUCCESS);
}

// -(12+12*b)/(b*23-a) >= 12*(123-a+b)
TEST_F(Precedence, ge_combo) {
	fprintf(stream, "-(12+12*b)/(b*23-a) >= 12*(123-a+b)");

	TESTVAL(EXIT_SUCCESS);
}

// a <
TEST_F(Precedence, lt_missing_term1) {
	fprintf(stream, "a<");

	TESTVAL(ERROR_SYN);
}

// < a
TEST_F(Precedence, lt_missing_term2) {
	fprintf(stream, "< a");

	TESTVAL(ERROR_SYN);
}

// a <=
TEST_F(Precedence, le_missing_term1) {
	fprintf(stream, "a <=");

	TESTVAL(ERROR_SYN);
}

// <= a
TEST_F(Precedence, le_missing_term2) {
	fprintf(stream, "<= a");

	TESTVAL(ERROR_SYN);
}

// a >
TEST_F(Precedence, gt_missing_term1) {
	fprintf(stream, "a >");

	TESTVAL(ERROR_SYN);
}

// > a
TEST_F(Precedence, gt_missing_term2) {
	fprintf(stream, "> a");

	TESTVAL(ERROR_SYN);
}

// a >=
TEST_F(Precedence, ge_missing_term1) {
	fprintf(stream, "a >=");

	TESTVAL(ERROR_SYN);
}

// >= a
TEST_F(Precedence, ge_missing_term2) {
	fprintf(stream, ">=a");

	TESTVAL(ERROR_SYN);
}

// /***************************************
// *
// *   COMPARATORS 2 ( ==, != )
// *
// ***************************************/

// a == b
TEST_F(Precedence, eq_id) {
	fprintf(stream, "a==b");

	TESTVAL(EXIT_SUCCESS);
}

// a == 12
TEST_F(Precedence, eq_id_int) {
	fprintf(stream, "a==12");

	TESTVAL(EXIT_SUCCESS);
}

// a == 12.23
TEST_F(Precedence, eq_id_float) {
	fprintf(stream, "a == 12.23");

	TESTVAL(EXIT_SUCCESS);
}

// a == true
TEST_F(Precedence, eq_id_bool) {
	fprintf(stream, "a == true");

	TESTVAL(EXIT_SUCCESS);
}

// 12 == 12
TEST_F(Precedence, eq_int) {
	fprintf(stream, "12 == 12");

	TESTVAL(EXIT_SUCCESS);
}

// 12.23 == 23.1
TEST_F(Precedence, eq_float) {
	fprintf(stream, "12.23 == 23.1");

	TESTVAL(EXIT_SUCCESS);
}

// true == false
TEST_F(Precedence, eq_bool) {
	fprintf(stream, "true == false");

	TESTVAL(EXIT_SUCCESS);
}


// a != b
TEST_F(Precedence, neq_id) {
	fprintf(stream, "a!=b");

	TESTVAL(EXIT_SUCCESS);
}

// a != 12
TEST_F(Precedence, neq_id_int) {
	fprintf(stream, "a!=12");

	TESTVAL(EXIT_SUCCESS);
}

// a != 12.23
TEST_F(Precedence, neq_id_float) {
	fprintf(stream, "a != 12.23");

	TESTVAL(EXIT_SUCCESS);
}

// a != true
TEST_F(Precedence, neq_id_bool) {
	fprintf(stream, "a != true");

	TESTVAL(EXIT_SUCCESS);
}

// 12 != 12
TEST_F(Precedence, neq_int) {
	fprintf(stream, "12 != 12");

	TESTVAL(EXIT_SUCCESS);
}

// 12.23 != 23.1
TEST_F(Precedence, neq_float) {
	fprintf(stream, "12.23 != 23.1");

	TESTVAL(EXIT_SUCCESS);
}

// true != false
TEST_F(Precedence, neq_bool) {
	fprintf(stream, "true != false");

	TESTVAL(EXIT_SUCCESS);
}


// a+12*(-b/12) == 12*(a-(b/-12))
TEST_F(Precedence, eq_combo1) {
	fprintf(stream, "a+12*(-b/12) == 12*(a-(b/-12))");

	TESTVAL(EXIT_SUCCESS);
}

// a+12*(-b/12) != 12*(a-(b/-12))
TEST_F(Precedence, neq_combo1) {
	fprintf(stream, "a+12*(-b/12) != 12*(a-(b/-12))");

	TESTVAL(EXIT_SUCCESS);
}

// a < b == b > a
TEST_F(Precedence, eq_combo2) {
	fprintf(stream, "a < b == b > a");

	TESTVAL(EXIT_SUCCESS);
}

// a < b != b > a
TEST_F(Precedence, neq_combo2) {
	fprintf(stream, "a < b != b > a");

	TESTVAL(EXIT_SUCCESS);
}

// TODO: fail cases

/***************************************
*
*   LOGICAL AND (&&)
*
***************************************/

// a && b
TEST_F(Precedence, and_id) {
	fprintf(stream, "a && b");

	TESTVAL(EXIT_SUCCESS);
}

// a && true
TEST_F(Precedence, and_id_bool) {
	fprintf(stream, "a && true");

	TESTVAL(EXIT_SUCCESS);
}

// true && false
TEST_F(Precedence, and_bool) {
	fprintf(stream, "true && false");

	TESTVAL(EXIT_SUCCESS);
}

// a < b && b > 10
TEST_F(Precedence, and_combo1) {
	fprintf(stream, "a < b && b > 10");

	TESTVAL(EXIT_SUCCESS);
}

// a == b && b > 10
TEST_F(Precedence, and_combo2) {
	fprintf(stream, "a == b && b > 10");

	TESTVAL(EXIT_SUCCESS);
}


// /***************************************
// *
// *   LOGICAL OR (||)
// *
// ***************************************/

// a || b
TEST_F(Precedence, or_id) {
	fprintf(stream, "a || b");

	TESTVAL(EXIT_SUCCESS);
}

// a || true
TEST_F(Precedence, or_id_bool) {
	fprintf(stream, "a || true");

	TESTVAL(EXIT_SUCCESS);
}

// false || false
TEST_F(Precedence, or_bool) {
	fprintf(stream, "true || false");

	TESTVAL(EXIT_SUCCESS);
}

// a < b || b > 10
TEST_F(Precedence, or_combo1) {
	fprintf(stream, "a < b || b > 10");

	TESTVAL(EXIT_SUCCESS);
}

// a == b || b > 10
TEST_F(Precedence, or_combo2) {
	fprintf(stream, "a == b || b > 10");

	TESTVAL(EXIT_SUCCESS);
}

TEST_F(Precedence, comma) {
	fprintf(stream, "a+a, b+b");
	TESTVAL(EXIT_SUCCESS);
	tklist_pop_front(tklist);
	s_parser->token = tklist_front(tklist);
	EXPECT_EQ(parse_expr(s_parser), EXIT_SUCCESS);
}


/* TEST_F(Precedence, fails) {
	EXPECT_TRUE(false);
} */

// todo: fail cases
