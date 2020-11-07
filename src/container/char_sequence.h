#ifndef IFJ_CHAR_SEQUENCE_H
#define IFJ_CHAR_SEQUENCE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct CharSequence *charseq_t;

/**
 * Allocates and initializes a char sequence
 * @return the initialized char sequence or NULL on error
 */
charseq_t charseq_init();

/**
 * Appends a character to the end of a char sequence, the char sequence may be resized if neccessary
 * @param charseq
 * @param c character to be appended
 * @return true on success, false on allocation error
 */
bool charseq_push_back(charseq_t charseq, int c);

/**
 * Getter for the char sequence length
 * @param charseq
 * @return actual length of the string in bytes (should be equal to strlen)
 */
size_t charseq_len(charseq_t charseq);

/**
 * Getter for the char sequence data
 * @param charseq
 * @return c string representing the char sequence
 */
char const *charseq_data(charseq_t charseq);

/**
 * Clear the char sequence data
 * @param charseq
 */
void charseq_clear(charseq_t charseq);

/**
 * Free the allocated char sequence
 * @param charseq
 * @note Invalidates all pointers pointing to char sequence data
 */
void charseq_free(charseq_t charseq);

#endif //IFJ_CHAR_SEQUENCE_H
