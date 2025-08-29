#include "test.h"

#include "evaluator.h"
#include "token_list.h"
#include "safe.h"

// Test case for simple addition: 3 + 4 = 7
void test_evaluate_addition(void) {
    // 1. Setup
    token_list_t *rpn_list = init_token_list();
    // ... (same setup code as before to create tokens for 3, 4, +)
    token_t *three = safe_malloc(sizeof(token_t)); three->type = TOKEN_INT; three->int_value = 3;
    token_t *four = safe_malloc(sizeof(token_t)); four->type = TOKEN_INT; four->int_value = 4;
    token_t *plus = safe_malloc(sizeof(token_t)); plus->type = TOKEN_OPERATOR; plus->operator = OP_ADDITION;
    append_token(rpn_list, three); append_token(rpn_list, four); append_token(rpn_list, plus);

    // 2. Execution
    token_t *result = evaluate_RPN(rpn_list);

    // 3. Assertion: Use your new ASSERT macro!
    ASSERT(result != NULL);
    if (result) {
        ASSERT(result->type == TOKEN_FLOAT);
        ASSERT(result->float_value == 7.0);
    }

    ASSERT(1 == 0); // something bad

    // 4. Teardown
    if (result) {
        free_token(result);
    }
}

// Another test case for a more complex expression
void test_evaluate_complex(void) {
    // Left as an exercise for you!
    // Try implementing the test for "5 * (1 + 2) - 3 = 12"
}

// This is our list of tests. It's just an array of test_case_t structs.
test_case_t evaluator_tests[] = {
    {test_evaluate_addition, "evaluator: simple addition (3+4)"},
    {test_evaluate_complex, "evaluator: complex expression (5*(1+2)-3)"},
    {NULL, NULL} // This NULL entry marks the end of the list.
};
