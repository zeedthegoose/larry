#include <math.h>
#include <stdio.h>

#include "safe.h"
#include "token_list.h"

// only supports (+, -, *, /, ^)
token_t *evaluate_RPN(token_list_t *list) {
    token_list_t *stack = init_token_list();

    token_t *token;
    while ((token = pop_token(list))) {

        switch (token->type) {
            case TOKEN_INT: {
                token->type = TOKEN_FLOAT; // store as float for uniformity
                token->float_value = (double)token->int_value;
                push_token(stack, token);
                break;
            }
            case TOKEN_FLOAT: {
                push_token(stack, token);
                break;
            }
            case TOKEN_OPERATOR: {
                token_t *b = pop_token(stack);
                token_t *a = pop_token(stack);

                if (!a || !b) {
                    fprintf(stderr, "Evaluator Error: Operator does not have "
                                    "two operands\n");
                    if (a) {
                        free_token(a);
                    }
                    if (b) {
                        free_token(b);
                    }
                    break;
                }

                double x = a->float_value;
                double y = b->float_value;

                double result = 0;
                switch (token->operator) {
                    case OP_ADDITION:
                        result = x + y;
                        break;
                    case OP_SUBTRACTION:
                        result = x - y;
                        break;
                    case OP_MULTIPLICATION:
                        result = x * y;
                        break;
                    case OP_DIVISION:
                        result = x / y;
                        break;
                    case OP_POWER:
                        result = pow(x, y);
                        break;
                    default:
                        fprintf(stderr, "Evaluator Error: Unknown operator\n");
                }

                free_token(a);
                free_token(b);

                token_t *result_token = safe_malloc(sizeof(token_t));
                result_token->type = TOKEN_FLOAT;
                result_token->float_value = result;
                push_token(stack, result_token);

                break;
            }
            default:
                fprintf(stderr,
                        "Evaluator Error: Unexpected token in evaluation\n");
        }
    }

    token_t *result = pop_token(stack);
    free_token_list(stack);
    free_token_list(list);
    return result;
}
