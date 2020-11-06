#include <stdio.h>

#include "scanner.h"
#include "scanner_states.h"
#include "char_sequence.h"

struct Scanner
{
	charseq_t charseq;
	token_t token; // used for referencing the current token between states
};

scanner_t scanner_init() {
	scanner_t scanner = calloc(1, sizeof(struct Scanner));
	if (scanner == NULL) {
		return NULL;
	}

	scanner->charseq = charseq_init();
	if (scanner->charseq == NULL) {
		free(scanner);
		return NULL;
	}

	scanner->token = NULL;
	return scanner;
}

void scanner_retrieve_token(scanner_t scanner, token_t token) {
	charseq_clear(scanner->charseq);
	scanner->token = token;

	scanner_state_t state = S_START;
	while (state != S_END) {
		state = state_map[state](scanner, getc(stdin));
	}

	// TODO: Add keyword table - Kevin
}

void scanner_free(scanner_t scanner) {
	if (scanner != NULL) {
		charseq_free(scanner->charseq);
		free(scanner);
	}
}
