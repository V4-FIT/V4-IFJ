#include <stdio.h>

#include "tokens.h"
#include "scanner_states.h"

void retrieve_token(token_t token) {
	scanner_state_t state = S_START;

	while (state != S_END) {
		state = foo_ptr_array[state](token, getc(stdin));
	}
}
