#ifndef TOKEN_H
#define TOKEN_H

#include "operator.h"

typedef enum {
    TOKEN_INVALID,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_DOT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_EOF,
} token_type_t;

typedef struct {
    token_type_t type;
    union {
        int int_value;             // number tokens
        double float_value;        // number tokens
        operator_type_t operator;  // operator token
        char *identifier;          // variable tokens
        char invalid_char;         // invalid token
    };
} token_t;

char *token_to_string(token_t *token);
char *token_to_value(token_t *token);
void free_token(token_t *token);

#endif // TOKEN_H
