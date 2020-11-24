#include <stdlib.h>

#include "parser.h"
#include "error.h"
#include "rules.h"

parser_t parser_init(FILE *stream) {
	parser_t parser = malloc(sizeof(struct parser));
	if (parser == NULL) {
		return NULL;
	}

	parser->scanner = scanner_init(stream);
	if (parser->scanner == NULL) {
		free(parser);
		return NULL;
	}

	parser->symtable = symtable_init();
	if (parser->symtable == NULL) {
		scanner_free(parser->scanner);
		free(parser);
		return NULL;
	}

	return parser;
}

int parser_parse(parser_t parser) {
	int res = rule_root(parser);

	// SEM - check for undefined functions
	if (symtable_undefined_funcs(parser->symtable)) {
		res = ERROR_DEFINITION;
	}

	return res;
}

void parser_free(parser_t parser) {
	scanner_free(parser->scanner);
	symtable_free(parser->symtable);
	free(parser);
}
