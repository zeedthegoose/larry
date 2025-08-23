#include "token.h"
#include "trie.h"

#include "lexer.h"
#include "dispatch.h"
#include "safe.h"

trie_node_t *insert_node(trie_node_t *root, const char *str, trie_payload_type_t type) {
    trie_node_t *node = root;
    unsigned char *p = (unsigned char *) str;

    while (*p != '\0') {
        // printf("inserting: %d\n", *p);
        if (!node->children[(int) *p]) {
            node->children[(int) *p] = safe_calloc(1, sizeof(trie_node_t));
        }
        node = node->children[(int) *p];

        p++;
    }

    node->payload_type = type;

    return node;
}

void common(trie_node_t *root) {
    // Insert all operators
    insert_node(root, "+", TRIE_OP)->operator_type = OP_ADDITION;
    insert_node(root, "-", TRIE_OP)->operator_type = OP_SUBTRACTION;
    insert_node(root, "*", TRIE_OP)->operator_type = OP_MULTIPLICATION;
    insert_node(root, "/", TRIE_OP)->operator_type = OP_DIVISION;
    insert_node(root, "^", TRIE_OP)->operator_type = OP_POWER;

    // Mark identifier starting chars
    for (int c = 'a'; c <= 'z'; c++) {
        char buf[2] = { c, '\0' };
        insert_node(root, buf, TRIE_DISPATCH)->dispatch_handler = read_identifier;
    }
    for (int c = 'A'; c <= 'Z'; c++) {
        char buf[2] = { c, '\0' };
        insert_node(root, buf, TRIE_DISPATCH)->dispatch_handler = read_identifier;
    }
    insert_node(root, "_", TRIE_DISPATCH)->dispatch_handler = read_identifier;

    // Mark starting digits for ints
    for (int c = '0'; c <= '9'; c++) {
        char buf[2] = { c, '\0' };
        insert_node(root, buf, TRIE_DISPATCH)->dispatch_handler = read_int;
    }

    // Mark starting digits for floats
    for (int c = '0'; c <= '9'; c++) {
        char buf[3] = { '.', c, '\0' };
        insert_node(root, buf, TRIE_DISPATCH)->dispatch_handler = read_float;
    }

    // tokens
    insert_node(root, "(", TRIE_TOKEN)->token_type = TOKEN_LPAREN;
    insert_node(root, ")", TRIE_TOKEN)->token_type = TOKEN_RPAREN;
    insert_node(root, ".", TRIE_TOKEN)->token_type = TOKEN_DOT;

    // comment nodes
    insert_node(root, "//", TRIE_TOKEN)->payload_type = TRIE_COMMENT_SINGLE;
    insert_node(root, "/*", TRIE_TOKEN)->payload_type = TRIE_COMMENT_MULTI;
    
    // end tokens
    insert_node(root, ";", TRIE_TOKEN)->token_type = TOKEN_END;

    char buf[2] = { '\0', '\0' };
    // whitespace tokens
    buf[0] = ' ';
    insert_node(root, buf, TRIE_WHITESPACE);
    buf[0] = '\t';
    insert_node(root, buf, TRIE_WHITESPACE);
}

trie_node_t *init_trie_file() {
    trie_node_t *trie = safe_calloc(1, sizeof(trie_node_t));

    common(trie);

    char buf[2] = { '\n', '\0' };
    insert_node(trie, buf, TRIE_WHITESPACE);
    buf[0] = EOF;
    insert_node(trie, buf, TRIE_TOKEN)->token_type = TOKEN_EOF;

    return trie;
}

trie_node_t *init_trie_repl() {
    trie_node_t *trie = safe_calloc(1, sizeof(trie_node_t));

    common(trie);

    // eof token
    // dispatch is needed to consume the character
    // default behavior is to not consume or lookahead after eof is encountered
    char buf[2] = { '\n', '\0' };
    insert_node(trie, buf, TRIE_DISPATCH)->dispatch_handler = consume_eof;

    return trie;
}

void free_trie(trie_node_t *root) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < TRIE_CHILDREN_COUNT; i++) {
        free_trie(root->children[i]);
    }

    free(root);
}
