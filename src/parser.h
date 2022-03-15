#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"

#include <stdint.h>

enum ast_node_type_e {
    AST_ADD, AST_SUBTRACT, AST_MULTIPLY, AST_DIVIDE,
    AST_INT_LITERAL,
    AST_EOF, AST_INVALID
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

void parser_init(struct parser_t* parser, struct scanner_t* scanner);
void parse_tokens(struct parser_t* parser);
int32_t parser_evaluate(struct ast_node_t* node);

int32_t ast_type_from_token(int32_t token_type);
struct token_t* parser_current_token(struct parser_t* parser);
struct token_t* parser_next_token(struct parser_t* parser);
struct ast_node_t* ast_binary_expr(struct parser_t* parser);
struct ast_node_t* ast_primary_expr(struct parser_t* parser);

struct ast_node_t* ast_node(int32_t type, int32_t value, struct ast_node_t* left, struct ast_node_t* right);
struct ast_node_t* ast_node_unary(int32_t type, int32_t value, struct ast_node_t* left);
struct ast_node_t* ast_node_leaf(int32_t type, int32_t value);

#endif
