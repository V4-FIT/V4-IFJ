#include "test_syntax_file.h"

#define EXAMPLES_PATH "go_files/syntactic_fail"

TEST_FILE(func_type_param_double, ERROR_SYN)

TEST_FILE(func_type_param_invalid, ERROR_SYN)

TEST_FILE(func_type_return_double, ERROR_SYN)

TEST_FILE(func_type_return_invalid, ERROR_SYN)

TEST_FILE(func_type_return_invalid_2, ERROR_SYN)

TEST_FILE(prolog_double, ERROR_SYN)

TEST_FILE(prolog_invalid, ERROR_SYN)

TEST_FILE(prolog_noeol, ERROR_SYN)
