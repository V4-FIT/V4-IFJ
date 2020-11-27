#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "error.h"
#include "rules.h"
#include "precedence.h"


int parse(FILE *stream) {
	scanner_t scanner = scanner_init(stream);
	if (scanner == NULL) {
		return ERROR_MISC;
	}

	int res = rule_root(scanner);

	scanner_free(scanner);
	return res;
}
