/**
 * @file parser.c
 * @author Adrián Kálazi, Kevin Lackó
 * @brief Parser
 * @date 2020-12-08
 *
 */

#include "parser.h"

#include "error.h"
#include "generator.h"
#include "rules.h"

const char *stmt2str_map[] = {"default", "define", "assign", "call", "if", "for", "return"};

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

	parser->blockcounter = flist_init(sizeof(struct BlockCounter));
	if (parser->blockcounter == NULL) {
		flist_free(parser->return_id_list);
		symtable_free(parser->symtable);
		free(parser);
		return NULL;
	}

	parser->else_id = flist_init(sizeof(unsigned long));
	if (parser->else_id == NULL) {
		flist_free(parser->blockcounter);
		flist_free(parser->return_id_list);
		symtable_free(parser->symtable);
		free(parser);
		return NULL;
	}

	parser->assigned_ids_map = hmap_init(53, 1);
	if (parser->assigned_ids_map == NULL) {
		flist_free(parser->blockcounter);
		flist_free(parser->return_id_list);
		symtable_free(parser->symtable);
		flist_free(parser->else_id);
		free(parser);
	}

	parser->tklist = tklist;
	parser->token = tklist_front(tklist);
	parser->first_pass = true;

	gen_init();
	return parser;
}

int parser_setup(parser_t parser) {
	parser->tkit = tklist_begin(parser->tklist);
	if (!tklist_it_valid(parser->tkit)) {
		return ERROR_SYN;
	}
	parser->token = tklist_get(parser->tkit);
	parser->tkit2 = tklist_it_next(parser->tkit);
	if (!tklist_it_valid(parser->tkit2)) {
		return ERROR_SYN;
	}
	parser->token_second = tklist_get(parser->tkit2);
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
	gen_finish();

	symtable_free(parser->symtable);
	flist_free(parser->else_id);
	flist_free(parser->return_id_list);
	flist_free(parser->blockcounter);
	hmap_free(parser->assigned_ids_map);
	free(parser);
}
