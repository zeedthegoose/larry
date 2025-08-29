#define TEST_IMPLEMENTATION
#include "test.h"

// Bring in the lists of tests from other files
extern test_case_t evaluator_tests[];

test_case_t* TEST_SUITES[] = {
    evaluator_tests,
    NULL
};
