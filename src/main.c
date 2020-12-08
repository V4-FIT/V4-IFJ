/**
 * @file main.c
 * @author Adrián Kálazi, Kevin Lackó
 * @brief
 * @date 2020-12-08
 *
 */

#include "scanner.h"
#include "parser.h"
#include "error.h"

int main() {
	tklist_t tklist = tklist_init();
	int ret;
	if ((ret = scanner_scan(stdin, tklist)) != EXIT_SUCCESS) {
		goto EXIT_ERROR;
	}
	if ((ret = parser_parse(tklist)) != EXIT_SUCCESS) {
		goto EXIT_ERROR;
	}

EXIT_ERROR:
	tklist_free(tklist);
	return ret;
}
