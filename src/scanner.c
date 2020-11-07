#include <stdio.h>

#include "scanner.h"
#include "scanner_states.h"
#include "char_sequence.h"

// Private

struct Scanner
{
	FILE *stream;
	charseq_t charseq;
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

char *get_buf_escape(scanner_t scanner) {
	return scanner->buf_escape;
}

// Public

scanner_t scanner_init(FILE *stream) {
	scanner_t scanner = calloc(1, sizeof(struct Scanner));
	if (scanner == NULL) {
		return NULL;
	}

	scanner->charseq = charseq_init();
	if (scanner->charseq == NULL) {
		free(scanner);
		return NULL;
	}

	scanner->stream = stream;
	scanner->token = NULL;
	return scanner;
}

void scanner_retrieve_token(scanner_t scanner, token_t token) {
	charseq_clear(scanner->charseq);
	scanner->token = token;

	scanner_state_t state = S_START;
	while (state != S_END) {
		state = state_map[state](scanner, getc(scanner->stream));
	}

	// TODO: Add keyword table - Kevin
}

void scanner_free(scanner_t scanner) {
	if (scanner != NULL) {
		charseq_free(scanner->charseq);
		free(scanner);
	}
}
