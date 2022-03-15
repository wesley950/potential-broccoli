#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"

#include <stdint.h>

enum ast_node_type_e {
    AST_ADD, AST_SUBTRACT, AST_MULTIPLY, AST_DIVIDE,
    AST_INTEGER_LITERAL
};

struct ast_node_t {
    int32_t type;
    int32_t value;

    struct ast_node_t* left;
    struct ast_node_t* right;
};

struct parser_t {
    struct scanner_t* scanner;
    int32_t token;

    struct ast_node_t* ast_root;
};



struct ast_node_t* ast_node(int32_t type, int32_t value, struct ast_node_t* left, struct ast_node_t* right);
struct ast_node_t* ast_node_unary(int32_t type, int32_t value, struct ast_node_t* left);
struct ast_node_t* ast_node_leaf(int32_t type, int32_t value);

struct ast_node_t* parse_binary_expr();

#endif
