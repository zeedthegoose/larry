#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "input_stream.h"
#include "lexer.h"
#include "token_list.h"
#include "trie.h"
#include "shunting_yard.h"
#include "evaluator.h"

void run(token_list_t *tokens) {
    token_list_t *postfix = shunting_yard(tokens);

    // print_token_list(postfix);

    token_t *result = evaluate_RPN(postfix);

    if (result == NULL) {
        printf("bad result from evaluating\n");
        return;
    }

    char *result_str = token_to_value(result);
    // print the result
    printf("%s\n", result_str);
    free(result_str);

    free_token(result);
}

int run_file(char *path) {
    printf("Running file: %s\n", path);
    int fp = open(path, O_RDONLY);

    input_stream_t *input = init_stream(fp, 1024);
    trie_node_t *tree = init_trie_file();

    token_list_t *tokens = tokenize(input, tree);
    print_token_list(tokens);

    run(tokens);

    free_stream(input);
    free_trie(tree);

    return 0;
}

int run_repl() {
    input_stream_t *input = init_stream(STDIN_FILENO, 1024);
    trie_node_t *tree = init_trie_repl();

    while (1) {
        printf("\033[1;35m> \033[0m");
        fflush(stdout);

        // exit on eof on empty line
        int c = peek_char(input);
        if (c == EOF) {
            break;
        }

        token_list_t *tokens = tokenize(input, tree);
        print_token_list(tokens);

        run(tokens);
    }

    free_stream(input);
    free_trie(tree);

    return 0;
}
