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

////// Actual rules

static int rule_functions(scanner_t scanner) {
	return EXIT_SUCCESS;
}

static int rule_prolog(scanner_t scanner) {
	EXPECT_TERMINAL(TK_KEYW_PACKAGE);
	EXPECT_TERMINAL(TK_KEYW_MAIN);
	EXPECT_TERMINAL(TK_EOL);

	return EXIT_SUCCESS;
}

static int rule_program(scanner_t scanner) {
	EXPECT_NONTERMINAL(rule_prolog);
	EXPECT_NONTERMINAL(rule_functions);
	EXPECT_TERMINAL(TK_EOF);

	return EXIT_SUCCESS;
}

int rule_root(scanner_t scanner) {
	return rule_program(scanner);
}
