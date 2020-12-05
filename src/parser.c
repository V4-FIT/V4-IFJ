#include "parser.h"

#include <stdlib.h>
#include <assert.h>

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

	parser->return_id_list = flist_init(sizeof(char *));
	if (parser->return_id_list == NULL) {
		symtable_free(parser->symtable);
		free(parser);
		return NULL;
	}

	parser->tklist = tklist;
	parser->token = tklist_front(tklist);
	parser->first_pass = true;

	return parser;
}

int parser_setup(parser_t parser) {
	parser->tkit = tklist_begin(parser->tklist);
	if (!tklist_it_valid(parser->tkit)) {
		return ERROR_SYN;
	}
	parser->token = tklist_get(parser->tkit);
	parser->tkit = tklist_it_next(parser->tkit);
	if (!tklist_it_valid(parser->tkit)) {
		return ERROR_SYN;
	}
	parser->token_second = tklist_get(parser->tkit);
	return EXIT_SUCCESS;
}

int parser_parse(tklist_t tklist) {
	parser_t parser = parser_init(tklist);

	int res;
	if ((res = rule_root(parser)) == EXIT_SUCCESS) {
		parser->first_pass = false;
		res = rule_root(parser);
	}

	parser_free(parser);
	return res;
}

void parser_free(parser_t parser) {
	symtable_free(parser->symtable);
	flist_free(parser->return_id_list);
	free(parser);
}
