#include "rules.h"
#include "error.h"

////// Macros

#define EXPECT_TERMINAL(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return ERROR_SYN; \
    } \
} while(0)

#define EXPECT_NONTERMINAL(_SUBFUNC) \
do { \
    int _err_retval = _SUBFUNC(scanner); \
    if (_err_retval != EXIT_SUCCESS) { \
        return _err_retval; \
    } \
} while(0)

////// Forward declarations

static int rule_program(scanner_t scanner);
static int rule_prolog(scanner_t scanner);
static int rule_functions(scanner_t scanner);
static int rule_function_n(scanner_t scanner); // 2x
static int rule_function(scanner_t scanner);
static int rule_params(scanner_t scanner);
static int rule_param_n(scanner_t scanner); // 2x
static int rule_param(scanner_t scanner); // 2x
static int rule_returnTypes(scanner_t scanner);
static int rule_type_n(scanner_t scanner); // 2x
static int rule_type(scanner_t scanner); // 4x
static int rule_statements(scanner_t scanner);
// Statements -> ε
static int rule_statement_n(scanner_t scanner); // 2x
static int rule_statement(scanner_t scanner); // 6x
static int rule_var_define(scanner_t scanner);
// Var_define -> ε
static int rule_assignment(scanner_t scanner);
// Assignment -> ε
static int rule_ids(scanner_t scanner);
static int rule_id_n(scanner_t scanner); // 2x
static int rule_expressions(scanner_t scanner);
static int rule_expression_n(scanner_t scanner); // 2x
static int rule_assignOp(scanner_t scanner); // 5x
static int rule_conditionals(scanner_t scanner);
static int rule_conditional_n(scanner_t scanner); // 3x
static int rule_conditional(scanner_t scanner); // 2x
static int rule_iterative(scanner_t scanner);
static int rule_functionCall(scanner_t scanner); // 2x
static int rule_funParams(scanner_t scanner);
static int rule_funParam_n(scanner_t scanner); // 2x
static int rule_funParam(scanner_t scanner); // 2x
static int rule_term(scanner_t scanner); // 2x
static int rule_literal(scanner_t scanner); // 2x
static int rule_return(scanner_t scanner);

////// Root

int rule_root(scanner_t scanner) {
	return rule_program(scanner);
}

////// Actual rule definitions

static int rule_program(scanner_t scanner) {
	EXPECT_NONTERMINAL(rule_prolog);
	EXPECT_NONTERMINAL(rule_functions);
	EXPECT_TERMINAL(TK_EOF);

	return EXIT_SUCCESS;
}

static int rule_prolog(scanner_t scanner) {
	EXPECT_TERMINAL(TK_KEYW_PACKAGE);
	EXPECT_TERMINAL(TK_KEYW_MAIN);
	EXPECT_TERMINAL(TK_EOL);

	return EXIT_SUCCESS;
}

static int rule_functions(scanner_t scanner) {
	return EXIT_SUCCESS;
}
