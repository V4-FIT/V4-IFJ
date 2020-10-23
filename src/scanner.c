#include <stdio.h>
#include <enums.h>


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
    S_STAR,
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


void RetrieveToken(t_token token){
    char c;
    scannerState state = S_START;

    while(){
        c = getc(stdin);
        switch (state){
            case S_START:
                switch (c){
                    case "\n":
                        token->type = TK_EOL;
                        return;
                    break;
                    case ")":
                        token->type = TK_R_PARANTHESIS;
                        return;
                    break;
                    case "(":
                        token->type = TK_L_PARANTHESIS;
                        return;
                    break;
                    case EOF:
                        token->type = TK_EOF;
                        return;
                    break;
                    case ",":
                        token->type = TK_SEPARATOR;
                        return;
                    break;
                    case ";":
                        token->type = TK_SEMICOLON;
                        return;
                    break;
                    case "}":
                        token->type = TK_R_CURLY;
                        return;
                    break;
                    case "{":
                        token->type = TK_L_CURLY;
                        return;
                    break;

                    case "|":
                        state = S_PIPE;
                    break;                    
                    case "&":
                        state = S_AMPERSAND;
                    break;
                    case ":":
                        state = S_COLON;
                    break;
                    case "=":
                        state = S_ASSIGN;
                    break;
                    case "<":
                        state = S_LESS;
                    break;
                    case ">":
                        state = S_GREATER;
                    break;
                    case "!":
                        state = S_NOT;
                    break;
                    case "+":
                        state = S_PLUS;
                    break;
                    case "-":
                        state = S_MINUS;
                    break;
                    case "*":
                        state = S_STAR;
                    break;
                    case "/":
                        state = S_SLASH;
                    break;
                    case "
                }

            break;
            case S_PIPE:
                if (c == '|'){
                    token->type = TK_OR;
                } else {
                    token->type = TK_ERROR;
                }
                return;
            break;
            case S_AMPERSAND:
                if (c == '&'){
                    token->type = TK_AND;
                } else {
                    token->type = TK_ERROR;
                }
                return;
            break;
            case S_COLON:
                if (c == '='){
                    token->type = TK_VAR_INNIT;
                } else {
                    token->type = TK_ERROR;
                }
                return;
            break;
            case S_ASSIGN:
                if (c == '='){
                    token->type = TK_EQUAL;
                } else {
                    token->type = TK_ASSIGN;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_LESS:
                if (c == '='){
                    token->type = TK_LESS_EQUAL;
                } else {
                    token->type = TK_LESS;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_GREATER:
                if (c == '='){
                    token->type = TK_GREATER_EQUAL;
                } else {
                    token->type = TK_GREATER;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_NOT:
                if (c == '='){
                    token->type = TK_NOT_EQUAL;
                } else {
                    token->type = TK_NOT;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_PLUS:
                if (c == '='){
                    token->type = TK_INC;
                } else {
                    token->type = TK_PLUS;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_MINUS:
                if (c == '='){
                    token->type = TK_DEC;
                } else {
                    token->type = TK_MINUS;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_STAR:
                if (c == '='){
                    token->type = TK_TIMES;
                } else {
                    token->type = TK_STAR;
                    ungetc(c,stdin);
                }
                return;
            break;
            case S_SLASH:
                switch (c){
                    case '=':
                        token->type = TK_DIVIDE;
                        return;
                    break;
                    case '/':
                        state = S_SL_COMMENT;
                    break;
                    case '*':
                        state = S_ML_COMMENT1;
                    break;
                    default:
                        token->type = TK_SLASH;
                        ungetc(c,stdin);
                    break;
                }
                return;
            break;
            case S_SL_COMMENT:
                switch (c):
                    case EOF:
                        unget(c,stdin);
                    case '\n': //dont want to send EOL token here since it belongs to comment
                        state = S_START;
                    break;
            break;
            case S_ML_COMMENT1:
                if (c == EOF){
                    token->type = TK_ERROR;
                    return;
                } else if (c == '*'){
                    state = S_ML_COMMENT2;
                }
            break;
            case S_ML_COMMENT2:
                if (c == EOF){
                    token->type = TK_ERROR;
                    return;
                } else if (c == '/'){
                    state = S_START;
                    //not sending tokens for comment
                } else {
                    state = S_ML_COMMENT1;
                }
            break;
        }
    }
}
