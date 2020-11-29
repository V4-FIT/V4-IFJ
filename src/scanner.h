#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include <stdio.h>

#include "tokens.h"
#include "forward_list.h"

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
 * @brief	Scans the stream and populates token_list
 * @param	stream 
 * @param	token_list 
 * @return	return code: 0 - success, 1 - lexical error, 99 - misc error
*/
int scanner_scan(FILE *stream, tklist_t token_list);

/**
 * Free the allocated scanner
 * @param scanner
 */
void scanner_free(scanner_t scanner);

#endif //IFJ_SCANNER_H
