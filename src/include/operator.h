#ifndef OPERATOR_H
#define OPERATOR_H

typedef enum {
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_POWER,
} operator_type_t;

typedef enum {
    LEFT_ASSOCIATIVE,
    RIGHT_ASSOCIATIVE,
} associativity_t;

typedef struct {
    char *op;
    int precedence;
    associativity_t associativity;
} operator_info_t;

static const operator_info_t operators[] = {
    { "+", 2, LEFT_ASSOCIATIVE },
    { "-", 2, LEFT_ASSOCIATIVE },
    { "*", 3, LEFT_ASSOCIATIVE },
    { "/", 3, LEFT_ASSOCIATIVE },
    { "^", 4, RIGHT_ASSOCIATIVE },
    { 0,   0, LEFT_ASSOCIATIVE }, // sentinel
};

#endif // OPERATOR_H
