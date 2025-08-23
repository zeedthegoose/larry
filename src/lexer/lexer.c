#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "token.h"
#include "lexer.h"
#include "token_list.h"
#include "trie.h"
#include "input_stream.h"

#include "safe.h"

token_t *create_token(token_type_t type) {
    token_t *token = safe_malloc(sizeof(token_t));
    token->type = type;

    return token;
}

token_t *create_operator_token(operator_type_t op) {
    token_t *token = safe_malloc(sizeof(token_t));
    token->type = TOKEN_OPERATOR;
    token->operator = op;

    return token;
}

token_t *create_invalid_token(char c) {
    token_t *token = safe_malloc(sizeof(token_t));
    token->type = TOKEN_INVALID;
    token->invalid_char = c;

    return token;
}

void skip_single_line_comment(input_stream_t *in) {
    int c = peek_char(in);
    while (c != EOF && c != '\n') {
        next_char(in);
        c = peek_char(in);
    }
}

void skip_multi_line_comment(input_stream_t *in) {
    int prev = 0;
    int c = next_char(in);
    while (c != EOF) {
        if (prev == '*' && c == '/') {
            break; // end of comment
        }
        prev = c;
        c = next_char(in);
    }
}

token_t *next_token(input_stream_t *in, trie_node_t *root) {
    unsigned char c = peek_char(in);

    trie_node_t *node = root;
    trie_node_t *next = node->children[(int) c];

    while (next != NULL) {
        node = next;

        // skip whitespace
        if (node->payload_type == TRIE_WHITESPACE) {
            node = root;
        } else if (node->payload_type == TRIE_DISPATCH) {
            // call dispatch
            return node->dispatch_handler(in);
        } else if (node->payload_type == TRIE_COMMENT_SINGLE) {
            skip_single_line_comment(in);
            node = root;
        } else if (node->payload_type == TRIE_COMMENT_MULTI) {
            skip_multi_line_comment(in);
            node = root;
        } else if (c == (unsigned char) EOF) {
            // early exit for end tokens
            return create_token(TOKEN_EOF);
        }

        // move to next char
        next_char(in);
        c = peek_char(in);
        next = node->children[(int) c];
    }

    if (node->payload_type == TRIE_TOKEN) {
        return create_token(node->token_type);
    } else if (node->payload_type == TRIE_OP) {
        return create_operator_token(node->operator_type);
    }

    next_char(in);
    return create_invalid_token(c);
}

token_list_t *tokenize(input_stream_t *input, trie_node_t *tree) {
    token_list_t *tokens = init_token_list();

    while (1) {
        token_t *token = next_token(input, tree);

        if (token == NULL) {
            fprintf(stderr, "error: lex error\n");
            exit(EXIT_FAILURE);
        }

        append_token(tokens, token);
        // print_token_list(tokens);
        if (token->type == TOKEN_EOF) {
            break;
        }
    }

    return tokens;
}
