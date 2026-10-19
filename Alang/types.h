#include <stdbool.h>

#pragma once

// A language token types
enum TOKEN {
    TK_PLUS,
    TK_MINUS,
    TK_STAR,
    TK_SLASH,
    TK_INTLIT
};

// A language token struct
typedef struct {
    enum TOKEN token;
    int value;
} token_t;

// Token table
typedef struct {
    token_t *tokens;
    int tokenc;
} token_table_t;

// AST node types
enum AST_EXPR {
    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV
};

typedef struct _ast_node ast_node_t;

// AST node data
typedef union {
    enum AST_EXPR expr;
    int intval;
} ast_data_t;

// Basic AST node
typedef struct {
    ast_data_t data;

    ast_node_t left;
    ast_node_t right;
} ast_node_t;

typedef struct {
    ast_node_t *children;
    int childc;
} ast_tree_t;