#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "error.h"

#pragma once

// Initializes an Abstract Syntax Tree.
static ast_tree_t ast_tree_init() {
    ast_tree_t tree;

    tree.children = (ast_node_t*)malloc(0);
    tree.childc = 0;

    return tree;
}

// Generates an Abstract Syntax Node from a token.
static ast_node_t gen_ast(token_table_t tab, int *i) {
    token_t tok = tab.tokens[*i];

    ast_node_t node;

    switch (tok.token) {
        case TK_PLUS: {
            node.data.expr = AST_ADD;
            break;
        }
        case TK_MINUS: {
            node.data.expr = AST_SUB;
            break;
        }
        case TK_STAR: {
            /*
            * Pointers
            */

            node.data.expr = AST_MUL;
            break;
        }
        case TK_SLASH: {
            node.data.expr = AST_DIV;
        }
        case TK_INTLIT: {
            node.data.intval = tok.value;
        }
    }
}