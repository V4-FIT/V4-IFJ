#include "parser.h"
#include "error.h"

int main() {
	parser_t parser = parser_init(stdin);
	if (parser == NULL) {
		return ERROR_MISC;
	}

	int res = parser_parse(parser);

	parser_free(parser);

	return res;
}
