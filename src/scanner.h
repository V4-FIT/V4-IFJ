#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H

#include "tokens.h"

typedef struct Scanner *scanner_t;

scanner_t scanner_init();

void scanner_retrieve_token(scanner_t scanner, token_t token);

void scanner_free(scanner_t scanner);

#endif //IFJ_SCANNER_H
