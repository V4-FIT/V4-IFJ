#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include <stdio.h>

#include "tokens.h"

#define ESCAPE_SEQUENCE_BUFFER_SIZE 3

typedef struct Scanner *scanner_t;

/**
 * Initialises the scanner
 * @param stream file stream to scan from
 * @return the initialized scanner or NULL on error
 */
scanner_t scanner_init(FILE *stream);

/**
 * Updates the token data and returns a pointer to it
 * @param scanner initialized scanner
 * @return token pointer with updated data
 */
token_t scanner_next_token(scanner_t scanner);

/**
 * Return a pointer to the scanner token
 * @param scanner initialized scanner
 * @return token pointer
 * @note this function DOES NOT update the token values
 */
token_t scanner_token(scanner_t scanner);

/**
 * Free the allocated scanner
 * @param scanner
 */
void scanner_free(scanner_t scanner);

#endif //IFJ_SCANNER_H
