#include "parser.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

void parser_init(struct parser_t *parser, struct scanner_t *scanner)
{
    parser->scanner = scanner;
    parser->token = 0;
    parser->ast_root = NULL;
}

void parse_tokens(struct parser_t *parser)
{
    parser->ast_root = ast_binary_expr(parser);
}

int32_t parser_evaluate(struct ast_node_t *node)
{
    int32_t left_value = 0;
    int32_t right_value = 0;

    if (node->left != NULL)
        left_value = parser_evaluate(node->left);
    if (node->right != NULL)
        right_value = parser_evaluate(node->right);

    switch (node->type)
    {
    case AST_INT_LITERAL:
        return node->value;

    case AST_ADD:
        return left_value + right_value;

    case AST_SUBTRACT:
        return left_value - right_value;

    case AST_MULTIPLY:
        return left_value * right_value;

    case AST_DIVIDE:
        return left_value / right_value;
    }

    return 0;
}

int32_t ast_type_from_token(int32_t token_type)
{
    switch (token_type)
    {
    case TK_ADD:
        return AST_ADD;

    case TK_SUBTRACT:
        return AST_SUBTRACT;

    case TK_MULTIPLY:
        return AST_MULTIPLY;

    case TK_DIVIDE:
        return AST_DIVIDE;

    case TK_INT_LITERAL:
        return AST_INT_LITERAL;

    case TK_EOF:
        return AST_EOF;
    }

    return AST_INVALID;
}

struct token_t *parser_current_token(struct parser_t *parser)
{
    if (parser->token < parser->scanner->token_count)
    {
        return &parser->scanner->tokens[parser->token];
    }

    return NULL;
}

struct token_t *parser_next_token(struct parser_t *parser)
{
    parser->token++;
    return parser_current_token(parser);
}

struct ast_node_t *ast_binary_expr(struct parser_t *parser)
{
    struct ast_node_t *left_token = ast_primary_expr(parser);
    struct token_t *next_token = parser_next_token(parser);

    if (next_token == NULL || next_token->type == TK_EOF)
    {
        return left_token;
    }

    int32_t ast_node_type = ast_type_from_token(next_token->type);

    parser_next_token(parser);
    struct ast_node_t *right_token = ast_binary_expr(parser);
    return ast_node(ast_node_type, 0, left_token, right_token);
}

struct ast_node_t *ast_primary_expr(struct parser_t *parser)
{
    struct token_t *current_token = parser_current_token(parser);
    int32_t ast_node_type = ast_type_from_token(current_token->type);

    switch (ast_node_type)
    {
    case AST_INT_LITERAL:
        return ast_node_leaf(ast_node_type, current_token->value);
    }

    LOG_ERROR("Expected token type: integer literal.");
    return ast_node_leaf(AST_INT_LITERAL, 0);
}

struct ast_node_t *ast_node(int32_t type, int32_t value, struct ast_node_t *left, struct ast_node_t *right)
{
    struct ast_node_t *node = (struct ast_node_t *)malloc(sizeof(struct ast_node_t));
    if (node == NULL)
    {
        LOG_ERROR("Unable to allocate memory");
        return NULL;
    }

    node->type = type;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

struct ast_node_t *ast_node_unary(int32_t type, int32_t value, struct ast_node_t *left)
{
    return ast_node(type, value, left, NULL);
}

struct ast_node_t *ast_node_leaf(int32_t type, int32_t value)
{
    return ast_node(type, value, NULL, NULL);
}
