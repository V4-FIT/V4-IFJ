#include "scanner.h"
#include "parser.h"
#include "error.h"

int main() {
	tklist_t tklist = tklist_init();
	int ret;
	if (!(ret = scanner_scan(stdin, tklist))) {
		goto EXIT_ERROR;
	}
	if (!(ret = parser_parse(tklist))) {
		goto EXIT_ERROR;
	}

EXIT_ERROR:
	tklist_free(tklist);
	return ret;
}
