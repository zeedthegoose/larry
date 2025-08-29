#ifndef SIMPLE_TEST_H
#define SIMPLE_TEST_H

#include <stdio.h>
#include <stdlib.h>

int test_pass_count;
int test_fail_count;

typedef void (*test_func_t)(void);
typedef struct {
    test_func_t func;
    const char *name;
} test_case_t;

extern test_case_t* TEST_SUITES[];

void record_failure(const char* file, int line, const char* condition_str);

#define ASSERT(condition)                                                      \
    do {                                                                       \
        if (condition) {                                                       \
            test_pass_count++;                                                 \
        } else {                                                               \
            test_fail_count++;                                                 \
            record_failure(__FILE__, __LINE__, #condition);                    \
        }                                                                      \
    } while (0)

#endif // SIMPLE_TEST_H

#ifdef TEST_IMPLEMENTATION

// failure logging

typedef struct {
    const char* file;
    int line;
    const char* condition_str;
} failure_record_t;

typedef struct failure_list {
    failure_record_t *record;
    struct failure_list *next;
} failure_list_node_t;

failure_list_node_t* failure_log;

void record_failure(const char* file, int line, const char* condition_str) {
    failure_list_node_t *record = failure_log;

    while (record->next != NULL) {
        record = record->next;
    }

    record->next = malloc(sizeof(failure_list_node_t));

    if (!record->next) {
        fprintf(stderr, "Failed to allocate memory for failure log!\n");
        exit(EXIT_FAILURE);
    }

    record->next->record = malloc(sizeof(failure_record_t));

    if (!record->next) {
        fprintf(stderr, "Failed to allocate memory for failure log!\n");
        exit(EXIT_FAILURE);
    }

    record->next->record->file = file;
    record->next->record->line = line;
    record->next->record->condition_str = condition_str;

    record->next->next = NULL;
}

#define C_B_WHITE   "\033[1;37m"
#define C_B_RED     "\033[1;31m"
#define C_B_GREEN   "\033[1;32m"
#define C_RESET     "\033[0m"

// main function to run all tests

int test_pass_count = 0;
int test_fail_count = 0;

int main() {
    failure_log = malloc(sizeof(failure_list_node_t));

    if (!failure_log) {
        fprintf(stderr, "Failed to allocate memory for failure log!\n");
        exit(EXIT_FAILURE);
    }
    failure_log->next = NULL;

    printf(C_B_WHITE "\nrunning tests\n\n" C_RESET);

    // run each suite
    for (int i = 0; TEST_SUITES[i] != NULL; i++) {
        test_case_t* current_suite = TEST_SUITES[i];

        for (int j = 0; current_suite[j].name != NULL; j++) {
            printf("%-*s", 60, current_suite[j].name);

            int failures_before_test = test_fail_count;
            current_suite[j].func();
            int num_failures = test_fail_count - failures_before_test;

            if (num_failures > 0) {
                printf("[ \033[1;31mfail\033[0m ]\n");
            } else {
                printf("[ \033[1;32mok\033[0m ]\n");
            }

            // print failures
            while (failure_log->next != NULL) {
                printf("  %s:%d: %s ... " C_B_RED "fail\n" C_RESET, failure_log->next->record->file, failure_log->next->record->line, failure_log->next->record->condition_str);
                failure_log->next = failure_log->next->next;
            }
        }
    }

    printf("\n");
    printf(C_B_WHITE "test results: " C_RESET);

    if (test_fail_count > 0) {
        printf(C_B_RED "fail" C_RESET);
    } else {
        printf(C_B_GREEN "ok" C_RESET);
    }

    printf(". %d passed; %d failed; finished in 0.00s\n", test_pass_count, test_fail_count);

    free(failure_log);

    return 0;
}

#endif // TEST_IMPLEMENTATION
