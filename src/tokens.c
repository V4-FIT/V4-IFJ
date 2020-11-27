#include "tokens.h"

#include <stdlib.h>
#include <string.h>

token_t token_copy(token_t token_to_copy) {
	token_t new_token = malloc(sizeof(struct Token));
	if (new_token == NULL) {
		return NULL;
	}

	if (token_to_copy->type == TK_IDENTIFIER || token_to_copy->type == TK_KEYW_MAIN) {
		size_t size = strlen(token_to_copy->param.s) + 1;
		new_token->param.s = malloc(size);
		if (new_token->param.s == NULL) {
			free(new_token);
			return NULL;
		}
		memcpy((void *)new_token->param.s, token_to_copy->param.s, size);
	} else {
		new_token->param = token_to_copy->param;
	}
	new_token->type = token_to_copy->type;

	return new_token;
}

void token_free(token_t token) {
	if (token->type == TK_IDENTIFIER || token->type == TK_KEYW_MAIN) {
		free((void *)token->param.s);
	}
	free(token);
}
