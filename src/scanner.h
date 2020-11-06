#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

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
 * Retrieves the next token
 * @param scanner initialized scanner
 * @param token token into which the new token data will be written
 */
void scanner_retrieve_token(scanner_t scanner, token_t token);

/**
 * Free the allocated scanner
 * @param scanner
 */
void scanner_free(scanner_t scanner);

#endif //IFJ_SCANNER_H
