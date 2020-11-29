#include <stdio.h>

#include "scanner.h"
#include "scanner_states.h"
#include "hash_map.h"
#include "error.h"

#define HMAP_BUCKET_COUNT 53

// Helper macro for mapping keywords to tokens and checking for allocation errors
#define MAP_KEYWORD_TOKEN(keyword, token)                                       \
	do {                                                                        \
		token_type_t tok = token;                                               \
		hmap_iterator_t it = hmap_insert(scanner->keyw_tok_map, keyword, &tok); \
		if (hmap_it_eq(it, hmap_end(scanner->keyw_tok_map))) {                  \
			charseq_free(scanner->charseq);                                     \
			hmap_free(scanner->keyw_tok_map);                                   \
			free(scanner);                                                      \
			return NULL;                                                        \
		}                                                                       \
	} while (0)

// Private

struct Scanner
{
	FILE *stream;
	charseq_t charseq;
	hmap_t keyw_tok_map;
	token_t token; // used for referencing the current token between states
	char buf_escape[ESCAPE_SEQUENCE_BUFFER_SIZE];
};

// Private (kinda) -> [not static] used only in scanner_states.c without public interface

FILE *get_stream(scanner_t scanner) {
	return scanner->stream;
}

token_t get_tok(scanner_t scanner) {
	return scanner->token;
}

charseq_t get_charseq(scanner_t scanner) {
	return scanner->charseq;
}

hmap_t get_keyw_tok_map(scanner_t scanner) {
	return scanner->keyw_tok_map;
}

char *get_buf_escape(scanner_t scanner) {
	return scanner->buf_escape;
}

// Public

scanner_t scanner_init(FILE *stream) {
	scanner_t scanner = calloc(1, sizeof(struct Scanner));
	if (scanner == NULL) {
		return NULL;
	}

	scanner->token = malloc(sizeof(struct Token));
	if (scanner->token == NULL) {
		free(scanner);
		return NULL;
	}

	scanner->charseq = charseq_init();
	if (scanner->charseq == NULL) {
		free(scanner->token);
		free(scanner);
		return NULL;
	}

	scanner->keyw_tok_map = hmap_init(HMAP_BUCKET_COUNT, sizeof(token_type_t));
	if (scanner->keyw_tok_map == NULL) {
		charseq_free(scanner->charseq);
		free(scanner->token);
		free(scanner);
		return NULL;
	}

	MAP_KEYWORD_TOKEN("package", TK_KEYW_PACKAGE);
	MAP_KEYWORD_TOKEN("func", TK_KEYW_FUNC);
	MAP_KEYWORD_TOKEN("main", TK_KEYW_MAIN);
	MAP_KEYWORD_TOKEN("return", TK_KEYW_RETURN);
	MAP_KEYWORD_TOKEN("if", TK_KEYW_IF);
	MAP_KEYWORD_TOKEN("else", TK_KEYW_ELSE);
	MAP_KEYWORD_TOKEN("for", TK_KEYW_FOR);
	MAP_KEYWORD_TOKEN("true", TK_KEYW_TRUE);
	MAP_KEYWORD_TOKEN("false", TK_KEYW_FALSE);
	MAP_KEYWORD_TOKEN("int", TK_KEYW_INT);
	MAP_KEYWORD_TOKEN("float64", TK_KEYW_FLOAT64);
	MAP_KEYWORD_TOKEN("string", TK_KEYW_STRING);
	MAP_KEYWORD_TOKEN("bool", TK_KEYW_BOOL);

	scanner->stream = stream;
	return scanner;
}

token_t scanner_next_token(scanner_t scanner) {
	charseq_clear(scanner->charseq);

	scanner_state_t state = S_START;
	while (state != S_END) {
		state = state_map[state](scanner, getc(scanner->stream));
	}

	return scanner->token;
}

// just a public wrapper function
token_t scanner_token(scanner_t scanner) {
	return get_tok(scanner);
}

int scanner_scan(FILE *stream, tklist_t token_list) {
	scanner_t scanner = scanner_init(stream);
	int ret = EXIT_SUCCESS;

	token_t tk;
	while ((tk = scanner_next_token(scanner))->type != TK_EOF) {
		if (tk->type == TK_ERROR) {
			ret = tk->param.i;
			goto SS_ERROR;
		}
		if (!tklist_push_back(token_list, tk)) {
			ret = ERROR_MISC;
			goto SS_ERROR;
		}
	}
	if (!tklist_push_back(token_list, tk)) {
		ret = ERROR_MISC;
		goto SS_ERROR;
	}

SS_ERROR:
	scanner_free(scanner);
	return ret;
}

void scanner_free(scanner_t scanner) {
	if (scanner != NULL) {
		charseq_free(scanner->charseq);
		hmap_free(scanner->keyw_tok_map);
		free(scanner->token);
		free(scanner);
	}
}
