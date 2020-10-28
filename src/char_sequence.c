#include <string.h>

#include "char_sequence.h"

////// Private

// define the struct here to hide the interface
struct CharSequence {
	char *data;
	size_t len;
	size_t mem_size;
};

/**
 * Resizes the allocated memory for the char sequence, if its length is equal to the allocated memory size
 * This allocates enough memory to add one character to the char sequence
 * @param charseq the char sequence to be resized
 * @return true on success, false on allocation error
 */
bool charseq_mayresize(charseq_t charseq) {
	if ((charseq->len + 1) == charseq->mem_size) {
		size_t oldsize = charseq->mem_size;
		charseq->mem_size *= GROWTH_FACTOR;
		charseq->data = realloc(charseq->data, sizeof(char) * (charseq->mem_size));
		if (charseq->data == NULL) {
			return false;
		}
		// clear the allocated memory
		memset(charseq->data + oldsize, 0, oldsize);
	}
	return true;
}

////// Public

charseq_t charseq_init() {
	charseq_t charseq = calloc(1, sizeof(struct CharSequence));
	if (charseq == NULL) {
		return NULL;
	}

	charseq->data = calloc(INITIAL_ALLOC_SIZE, sizeof(char));
	if (charseq->data == NULL) {
		return NULL;
	}

	charseq->mem_size = INITIAL_ALLOC_SIZE;
	return charseq;
}

bool charseq_add(charseq_t charseq, char c) {
	if (!charseq_mayresize(charseq)) {
		return false;
	}

	charseq->data[charseq->len++] = c;

	return true;
}

size_t charseq_len(charseq_t charseq) {
	return charseq->len;
}

char const *charseq_get(charseq_t charseq) {
	return charseq->data;
}

void charseq_free(charseq_t charseq) {
	if(charseq->data != NULL) {
		free(charseq->data);
	}
	free(charseq);
}
