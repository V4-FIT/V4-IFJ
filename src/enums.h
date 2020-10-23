#ifndef ENUMS
#define ENUMS

/*states of the scanner finite state machine*/
typedef enum {
    S_START,
    S_PIPE,
    S_AMPERSAND,
    S_COLON,
    S_ASSIGN,
    S_LESS,
    S_GREATER,
    S_NOT,
    S_PLUS,
    S_MINUS,
    S_START,
    S_SLASH,
    S_ML_COMMENT1,
    S_ML_COMMENT2,
    S_SL_COMMENT,
    S_STR_LIT,
    S_ESCAPE_SEQ,
    S_HEX1,
    S_HEX2,
    S_UNDERSCORE,
    S_ZERO,
    S_DEC_LIT,
    S_FLOAT_SCI_LIT,
    S_FLOAT_LIT,
    S_FLOAT_EXP,
    S_FLOAT_POINT,
    S_HEX_LIT,
    S_OCT_LIT,
    S_BIN_LIT
} scannerStates;


/*tokens produced by scanner*/
typedef enum {
    TK_STR_LIT,
    TK_EOL,
    TK_R_PARANTHESIS,
    TK_L_PARANTHESIS,
    TK_EOF,
    TK_SEPARATOR,
    TK_SEMICOLON,
    TK_L_CURLY,
    TK_R_CURLY,
    TK_OR,
    TK_AND,
    TK_VAR_INNIT,
    TK_ASSIGN,
    TK_EQUAL,
    TK_LESS,
    TK_LESS_EQUAL,
    TK_GREATER,
    TK_GREATER_EQUAL,
    TK_NOT,
    TK_NOT_EQUAL,
    TK_PLUS,
    TK_INC,
    TK_MINUS,
    TK_DEC,
    TK_STAR,
    TK_TIMES,
    TK_SLASH,
    TK_DIVIDES,
    TK_DEC_LIT,
    TK_FLOAT_SCI_LIT,
    TK_FLOAT_LIT,
    TK_HEX_LIT,
    TK_OCT_LIT,
    TK_BIN_LIT,
    TK_UNDERSCORE
    //TODO KEYWORD,IDENTIFIER,RESERVED 
} tokens;


/*data type to represent tokens created by scanner*/
typedef struct {
    tokens type;
    char* value;
} t_token;

#endif
