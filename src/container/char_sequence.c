#include "char_sequence.h"

#include <string.h>

////// Private

#define INITIAL_ALLOC_SIZE 16
#define GROWTH_FACTOR 2

// define the struct here to hide the interface
struct CharSequence
{
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
static bool charseq_mayresize(charseq_t charseq) {
	if (charseq == NULL) {
		return false;
	}

	if ((charseq->len + 1) == charseq->mem_size) {
		size_t oldsize = charseq->mem_size;
		charseq->mem_size *= GROWTH_FACTOR;
		void *data = realloc(charseq->data, sizeof(char) * (charseq->mem_size));
		if (data == NULL) {
			return false;
		} else {
			charseq->data = data;
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
		free(charseq);
		return NULL;
	}

	charseq->mem_size = INITIAL_ALLOC_SIZE;
	return charseq;
}

bool charseq_push_back(charseq_t charseq, int c) {
	if (!charseq_mayresize(charseq)) {
		return false;
	}

	charseq->data[charseq->len++] = (char)c;

	return true;
}

bool charseq_push_string(charseq_t charseq, const char *string) {
	for (const char *c = string; *c != '\0'; c++) {
		charseq_push_back(charseq, *c);
	}

	return true;
}

size_t charseq_len(charseq_t charseq) {
	return charseq->len;
}

char const *charseq_data(charseq_t charseq) {
	return charseq->data;
}

void charseq_clear(charseq_t charseq) {
	if (charseq == NULL || charseq->data == NULL) {
		return;
	}

	memset(charseq->data, '\0', charseq->len);
	charseq->len = 0;
}

void charseq_free(charseq_t charseq) {
	if (charseq == NULL) {
		return;
	}

	if (charseq->data != NULL) {
		free(charseq->data);
	}
	free(charseq);
}
