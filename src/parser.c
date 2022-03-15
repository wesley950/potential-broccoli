#include "parser.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

struct ast_node_t *ast_node(int32_t type, int32_t value, struct ast_node_t *left, struct ast_node_t *right)
{
    struct ast_node_t* node = (struct ast_node_t*)malloc(sizeof(struct ast_node_t));
    if (node == NULL) {
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
