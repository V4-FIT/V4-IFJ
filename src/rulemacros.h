#ifndef IFJ_RULEMACROS_H
#define IFJ_RULEMACROS_H

////// Defines

// for eps return a special return code
#define EPS_RETVAL 0xDEAD

////// Macros

#define REQUIRE_TERMINAL(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return ERROR_SYN; \
    } \
} while(0)

// expects keyword, if not then accepts everything but immediately returns
#define EXPECT_TERMINAL(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return EXIT_SUCCESS; \
    } \
} while(0)

// same as above but over a set
// returns silently in case none of the keywords match
#define EXPECT_TERMINAL_SET(_KEYWSET, _KWSETSIZE) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    int found = 0; \
    for (int i = 0; i < _KWSETSIZE; ++i) { \
        if (_tmp_token->type == _KEYWSET[i]) { \
            found = 1; \
        } \
    } \
    if (found == 0) { \
        return EXIT_SUCCESS; \
    } \
} while (0)

// expects keyword, if not then immediately returns and notifies parent
// WARNING: the parent must implement EXPECT_NONTERMINAL
// omitting this warning could result in an invalid return value from the compiler!
#define EXPECT_TERMINAL_NOTIFYPARENT(_KEYW) \
do { \
    token_t _tmp_token = scanner_retrieve_token(scanner); \
    if (_tmp_token->type != _KEYW) { \
        return EPS_RETVAL; \
    } \
} while(0)

// if a child returns eps -> syntactic error
#define REQUIRE_NONTERMINAL(_SUBFUNC) \
do { \
    int _err_retval = _SUBFUNC(scanner); \
    if (_err_retval != EXIT_SUCCESS) { \
        if (_err_retval == EPS_RETVAL) { \
            _err_retval = ERROR_SYN; \
        } \
        return _err_retval; \
    } \
} while(0)

// accepts EPS from child - silently ignores child eps return value
// the children of this call should implement EXPECT_TERMINAL_NOTIFYPARENT
#define EXPECT_NONTERMINAL(_SUBFUNC) \
do { \
    int _err_retval = _SUBFUNC(scanner); \
    if (_err_retval != EXIT_SUCCESS && _err_retval != EPS_RETVAL) { \
        return _err_retval; \
    } \
} while(0)

#endif //IFJ_RULEMACROS_H
