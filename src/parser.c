#include <stdlib.h>

#include "parser.h"
#include "error.h"
#include "rules.h"
#include "precedence.h"
#include "tokens.h"

parser_t parser_init(tklist_t tklist) {
	parser_t parser = malloc(sizeof(struct parser));
	if (parser == NULL) {
		return NULL;
	}

	parser->symtable = symtable_init();
	if (parser->symtable == NULL) {
		free(parser);
		return NULL;
	}

	parser->tklist = tklist;
	parser->token = tklist_front(tklist);

	return parser;
}

int parser_parse(tklist_t tklist) {
	parser_t parser = parser_init(tklist);
	int res = rule_root(parser);

	// SEM - check for undefined functions
	if (!res && symtable_undefined_funcs(parser->symtable)) {
		res = ERROR_DEFINITION;
	}

	parser_free(parser);
	return res;
}

void parser_free(parser_t parser) {
	symtable_free(parser->symtable);
	free(parser);
}
