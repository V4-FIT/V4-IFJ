#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "error.h"

token_t token;

#define EXPECT_TOKEN(_KEYW) \
do { \
    scanner_retrieve_token(scanner, token); \
    if (token->type != _KEYW) { \
        return ERROR_SYN; \
    } \
} while(0)

#define EXPECT_NONTERMINAL(_SUBFUNC) \
do { \
    ret = _SUBFUNC(scanner); \
    if (ret != EXIT_SUCCESS) { \
        return ret; \
    } \
} while(0)

static int functions(scanner_t scanner) {
	return EXIT_SUCCESS;
}

static int prolog(scanner_t scanner) {
	EXPECT_TOKEN(TK_KEYW_PACKAGE);
	EXPECT_TOKEN(TK_KEYW_MAIN);
	EXPECT_TOKEN(TK_EOL);

	return EXIT_SUCCESS;
}

static int program(scanner_t scanner) {
	int ret;

	EXPECT_NONTERMINAL(prolog);
	EXPECT_NONTERMINAL(functions);
	EXPECT_TOKEN(TK_EOF);

	return EXIT_SUCCESS;
}

int parse(FILE *stream) {
	token = malloc(sizeof(struct Token));
	if (token == NULL) {
		return ERROR_MISC;
	}

	scanner_t scanner = scanner_init(stream);
	if (scanner == NULL) {
		free(token);
		return ERROR_MISC;
	}

	int res = program(scanner);

	free(token);
	scanner_free(scanner);
	return res;
}
