#include "test_precedence.h"

/***************************************
*
*   BRACKETS
* 
***************************************/

// ()
TEST_F(Precedence, brackets_empty) {
    fscanf(stream, "()");

    TESTVAL(EXIT_SUCCESS);
}

// (a)
TEST_F(Precedence, brackets_id) {
    fscanf(stream, "(a)");

    TESTVAL(EXIT_SUCCESS);
}

// ((10))
TEST_F(Precedence, brackets_int) {
    fscanf(stream, "((10))");

    TESTVAL(EXIT_SUCCESS);
}

// (((12.3)))
TEST_F(Precedence, brackets_float) {
    fscanf(stream, "(((12.3)))");

    TESTVAL(EXIT_SUCCESS);
}

// ((("Hello")))
TEST_F(Precedence, brackets_string) {
    fscanf(stream, "((((\"Hello\")))");

    TESTVAL(EXIT_SUCCESS);
}

// ((((true))))
TEST_F(Precedence, brackets_bool) {
    fscanf(stream, "((((true))))");

    TESTVAL(EXIT_SUCCESS);
}


// (a
TEST_F(Precedence, brackets_rigt_missing) {
    fscanf(stream, "(a");

    TESTVAL(ERROR_SYN);
}

// a)
TEST_F(Precedence, brackets_left_missing) {
    fscanf(stream, "a)");

    TESTVAL(ERROR_SYN);
}

// (a)(a)
TEST_F(Precedence, brackets_missing_op) {
    fscanf(stream, "(a)(a)");

    TESTVAL(ERROR_SYN);
}


/***************************************
*
*   UNARY OPERATORS (+, -, !)
* 
***************************************/

// !a
TEST_F(Precedence, un_neg_id) {
    fscanf(stream, "!a");

    TESTVAL(EXIT_SUCCESS);
}

// !true 
TEST_F(Precedence, un_neg_true) {
    fscanf(stream, "!true");

    TESTVAL(EXIT_SUCCESS);
}

// !false
TEST_F(Precedence, un_neg_false) {
    fscanf(stream, "!false");

    TESTVAL(EXIT_SUCCESS);
}

// !!!a
TEST_F(Precedence, un_neg_id_multi) {
    fscanf(stream, "!!!a");

    TESTVAL(EXIT_SUCCESS);
}

// !(!a)
TEST_F(Precedence, un_neg_id_multi_brackets) {
    fscanf(stream, "!(!a)");

    TESTVAL(EXIT_SUCCESS);
}

// +a
TEST_F(Precedence, un_plus_id) {
    fscanf(stream, "+a");

    TESTVAL(EXIT_SUCCESS);
}

// -a
TEST_F(Precedence, un_minus_id) {
    fscanf(stream, "-a");

    TESTVAL(EXIT_SUCCESS);
}

// +10
TEST_F(Precedence, un_plus_int) {
    fscanf(stream, "+10");

    TESTVAL(EXIT_SUCCESS);
}

// -1O
TEST_F(Precedence, un_minus_int) {
    fscanf(stream, "-10");

    TESTVAL(EXIT_SUCCESS);
}

// ++--+-10
TEST_F(Precedence, un_plus_minus_combo1) {
    fscanf(stream, "++--+-10");

    TESTVAL(EXIT_SUCCESS);
}

// --+-++-10.12
TEST_F(Precedence, un_plus_minus_combo2) {
    fscanf(stream, "--+-++-10.12");

    TESTVAL(EXIT_SUCCESS);
}

// -(-(+(a)))
TEST_F(Precedence, un_plus_minus_combo3) {
    fscanf(stream, "--+-++-10.12");

    TESTVAL(EXIT_SUCCESS);
}

// +
TEST_F(Precedence, un_plus_missing_term) {
    fscanf(stream, "+");

    TESTVAL(ERROR_SYN);
}

// -
TEST_F(Precedence, un_minus_missing_term) {
    fscanf(stream, "-");

    TESTVAL(ERROR_SYN);
}

// +
TEST_F(Precedence, un_neg_missing_term) {
    fscanf(stream, "!");

    TESTVAL(ERROR_SYN);
}

/***************************************
*
*   BINARY OPERATORS 1 ( *, / )
* 
***************************************/

// a*b
TEST_F(Precedence, mul_id) {
    fscanf(stream, "a*b");

    TESTVAL(EXIT_SUCCESS);
}

// 10*10
TEST_F(Precedence, mul_int) {
    fscanf(stream, "10*10");

    TESTVAL(EXIT_SUCCESS);
}

// 10*a
TEST_F(Precedence, mul_id_int) {
    fscanf(stream, "10*a");

    TESTVAL(EXIT_SUCCESS);
}

// a*10.1
TEST_F(Precedence, mul_id_float) {
    fscanf(stream, "a*10.1");

    TESTVAL(EXIT_SUCCESS);
}

// 12.1*23.23
TEST_F(Precedence, mul_float) {
    fscanf(stream, "12.1*2.23");

    TESTVAL(EXIT_SUCCESS);
}

// a/b
TEST_F(Precedence, div_id) {
    fscanf(stream, "a*b");

    TESTVAL(EXIT_SUCCESS);
}

// 10/10
TEST_F(Precedence, div_int) {
    fscanf(stream, "10/10");

    TESTVAL(EXIT_SUCCESS);
}

// 10/a
TEST_F(Precedence, div_int_id) {
    fscanf(stream, "10/a");

    TESTVAL(EXIT_SUCCESS);
}

// 12.1/10.12
TEST_F(Precedence, div_float) {
    fscanf(stream, "12.1/10.12");

    TESTVAL(EXIT_SUCCESS);
}

// 12.1/a
TEST_F(Precedence, div_float_id) {
    fscanf(stream, "12.1/a");

    TESTVAL(EXIT_SUCCESS);
}

// a*b*c
TEST_F(Precedence, mul_id_multi) {
    fscanf(stream, "a*b*c");

    TESTVAL(EXIT_SUCCESS);
}

// a/b/c
TEST_F(Precedence, div_id_multi) {
    fscanf(stream, "a/b/c");

    TESTVAL(EXIT_SUCCESS);
}

// (10.0*a)/(12*b)
TEST_F(Precedence, div_mul_combo1) {
    fscanf(stream, "(10.0*a)/(+12*b)");

    TESTVAL(EXIT_SUCCESS);
} 

// 10.0*(a/12)*b
TEST_F(Precedence, div_mul_combo21) {
    fscanf(stream, "10.0*(a/-12)*b");

    TESTVAL(EXIT_SUCCESS);
} 

// 10.0*(a/(12*b))
TEST_F(Precedence, div_mul_combo3) {
    fscanf(stream, "10.0*(a/(12*b))");

    TESTVAL(EXIT_SUCCESS);
} 

// a*
TEST_F(Precedence, mul_missing_term) {
    fscanf(stream, "a*");

    TESTVAL(ERROR_SYN);
}

// b/
TEST_F(Precedence, div_missing_term) {
    fscanf(stream, "b/");

    TESTVAL(ERROR_SYN);
}

// /a
TEST_F(Precedence, div_missing_term2) {
    fscanf(stream, "/a");

    TESTVAL(ERROR_SYN);
}

// *b
TEST_F(Precedence, mul_missing_term2) {
    fscanf(stream, "*b");

    TESTVAL(ERROR_SYN);
}

/***************************************
*
*   BINARY OPERATORS 2 ( +, - )
* 
***************************************/

// a+a
TEST_F(Precedence, add_id) {
    fscanf(stream, "a+a");

    TESTVAL(EXIT_SUCCESS);
}

// a+1
TEST_F(Precedence, add_id_int) {
    fscanf(stream, "a+1");

    TESTVAL(EXIT_SUCCESS);
}

// a+12.1
TEST_F(Precedence, add_id_float) {
    fscanf(stream, "a+12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1+12
TEST_F(Precedence, add_int) {
    fscanf(stream, "1+12");

    TESTVAL(EXIT_SUCCESS);
}

// 12.12+12.12
TEST_F(Precedence, add_float) {
    fscanf(stream, "12.12+12.12");

    TESTVAL(EXIT_SUCCESS);
}

// a-b
TEST_F(Precedence, sub_id) {
    fscanf(stream, "a-b");

    TESTVAL(EXIT_SUCCESS);
}

// a-1
TEST_F(Precedence, sub_id_int) {
    fscanf(stream, "a-1");

    TESTVAL(EXIT_SUCCESS);
}

// a-12.1
TEST_F(Precedence, sub_id_float) {
    fscanf(stream, "a-12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1-12
TEST_F(Precedence, sub_int) {
    fscanf(stream, "1-12");

    TESTVAL(EXIT_SUCCESS);
}

// 12.12-23.1
TEST_F(Precedence, sub_float) {
    fscanf(stream, "12.12-23.1");

    TESTVAL(EXIT_SUCCESS);
}


// a+112-(b+12)
TEST_F(Precedence, add_sub_combo1) {
    fscanf(stream, "a+112-(b+12)");

    TESTVAL(EXIT_SUCCESS);
}

// (-(a-10)-(b+12))+(-12-a)
TEST_F(Precedence, add_sub_combo2) {
    fscanf(stream, " (-(a-10)-(b+12))+(-12-a)");

    TESTVAL(EXIT_SUCCESS);
}

// (a+b)/(b-a)-12
TEST_F(Precedence, add_sub_combo3) {
    fscanf(stream, "(a+b)/(b-a)-12");

    TESTVAL(EXIT_SUCCESS);
}

// a+(b/(b-a)-12)
TEST_F(Precedence, add_sub_combo4) {
    fscanf(stream, "a+(b/(b-a)-12)");

    TESTVAL(EXIT_SUCCESS);
}

// a+
TEST_F(Precedence, add_missing_term) {
    fscanf(stream, "a+");

    TESTVAL(ERROR_SYN);
}

// a-
TEST_F(Precedence, sub_missing_term) {
    fscanf(stream, "a-");

    TESTVAL(ERROR_SYN);
}


/***************************************
*
*   COMPARATORS 1 ( <, <=, >, >= )
* 
***************************************/

// a<b
TEST_F(Precedence, lt_id) {
    fscanf(stream, "a<b");

    TESTVAL(EXIT_SUCCESS);
}

// a<1
TEST_F(Precedence, lt_id_int) {
    fscanf(stream, "a<1");

    TESTVAL(EXIT_SUCCESS);
}

// a<12.1
TEST_F(Precedence, lt_id_float) {
    fscanf(stream, "a<12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1<12
TEST_F(Precedence, lt_int) {
    fscanf(stream, "1<12");

    TESTVAL(EXIT_SUCCESS);
}

// 1.23<1.1
TEST_F(Precedence, lt_float) {
    fscanf(stream, "1.23<1.1");

    TESTVAL(EXIT_SUCCESS);
}


// a<=b
TEST_F(Precedence, le_id) {
    fscanf(stream, "a<=b");

    TESTVAL(EXIT_SUCCESS);
}

// a<=1
TEST_F(Precedence, le_id_int) {
    fscanf(stream, "a<=1");

    TESTVAL(EXIT_SUCCESS);
}

// a<=12.1
TEST_F(Precedence, le_id_float) {
    fscanf(stream, "a<=12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1<=12
TEST_F(Precedence, le_int) {
    fscanf(stream, "1<=12");

    TESTVAL(EXIT_SUCCESS);
}

// 1.23<=1.1
TEST_F(Precedence, le_float) {
    fscanf(stream, "1.23<=1.1");

    TESTVAL(EXIT_SUCCESS);
}


// a>b
TEST_F(Precedence, gt_id) {
    fscanf(stream, "a>b");

    TESTVAL(EXIT_SUCCESS);
}

// a>1
TEST_F(Precedence, gt_id_int) {
    fscanf(stream, "a>1");

    TESTVAL(EXIT_SUCCESS);
}

// a>12.1
TEST_F(Precedence, gt_id_float) {
    fscanf(stream, "a>12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1>12
TEST_F(Precedence, gt_int) {
    fscanf(stream, "1>12");

    TESTVAL(EXIT_SUCCESS);
}

// 1.23>1.1
TEST_F(Precedence, gt_float) {
    fscanf(stream, "1.23>1.1");

    TESTVAL(EXIT_SUCCESS);
}


// a>=b
TEST_F(Precedence, ge_id) {
    fscanf(stream, "a>=b");

    TESTVAL(EXIT_SUCCESS);
}

// a>=1
TEST_F(Precedence, ge_id_int) {
    fscanf(stream, "a>=1");

    TESTVAL(EXIT_SUCCESS);
}

// a>=12.1
TEST_F(Precedence, ge_id_float) {
    fscanf(stream, "a>=12.1");

    TESTVAL(EXIT_SUCCESS);
}

// 1>=12
TEST_F(Precedence, ge_int) {
    fscanf(stream, "1>=12");

    TESTVAL(EXIT_SUCCESS);
}

// 1.23>=1.1
TEST_F(Precedence, ge_float) {
    fscanf(stream, "1.23>=1.1");

    TESTVAL(EXIT_SUCCESS);
}


// -(12+12*b/(b*23)-a) < 12*(123-a+b)
TEST_F(Precedence, lt_combo) {
    fscanf(stream, "-(12+12*b/(b*23)-a) < 12*(123-a+b)");

    TESTVAL(EXIT_SUCCESS);
}

// -(12+12*b)/(b*23-a) <= 12*123-a+b
TEST_F(Precedence, le_combo) {
    fscanf(stream, "-(12+12*b)/(b*23-a) <= 12*123-a+b");

    TESTVAL(EXIT_SUCCESS);
}

// -12+(12*b/b)*23-a) > 12*(123-a)+b)
TEST_F(Precedence, gt_combo) {
    fscanf(stream, "-12+(12*b/b)*23-a) > 12*(123-a)+b)");

    TESTVAL(EXIT_SUCCESS);
}

// -(12+12*b)/(b*23-a) >= 12*(123-a+b)
TEST_F(Precedence, ge_combo) {
    fscanf(stream, "-(12+12*b)/(b*23-a) >= 12*(123-a+b)");

    TESTVAL(EXIT_SUCCESS);
}

// a <
TEST_F(Precedence, lt_missing_term1) {
    fscanf(stream, "a<");

    TESTVAL(ERROR_SYN);
}

// < a
TEST_F(Precedence, lt_missing_term2) {
    fscanf(stream, "< a");

    TESTVAL(ERROR_SYN);
}

// a <=
TEST_F(Precedence, le_missing_term1) {
    fscanf(stream, "a <=");

    TESTVAL(ERROR_SYN);
}

// <= a
TEST_F(Precedence, le_missing_term2) {
    fscanf(stream, "<= a");

    TESTVAL(ERROR_SYN);
}

// a >
TEST_F(Precedence, gt_missing_term1) {
    fscanf(stream, "a >");

    TESTVAL(ERROR_SYN);
}

// > a
TEST_F(Precedence, gt_missing_term2) {
    fscanf(stream, "> a");

    TESTVAL(ERROR_SYN);
}

// a >=
TEST_F(Precedence, ge_missing_term1) {
    fscanf(stream, "a >=");

    TESTVAL(ERROR_SYN);
}

// >= a
TEST_F(Precedence, ge_missing_term2) {
    fscanf(stream, ">=a");

    TESTVAL(ERROR_SYN);
}

