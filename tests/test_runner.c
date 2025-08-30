#define TEST_IMPLEMENTATION
#include "test.h"

// Bring in the lists of tests from other files
extern test_suite_t evaluator_tests[];

test_suite_t TEST_SUITES[] = {
    {"evaluator test", evaluator_tests},
    {NULL},
};
