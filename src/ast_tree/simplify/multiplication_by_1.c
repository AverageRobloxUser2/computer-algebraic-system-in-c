#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

bool ast_node_simplify_multipliaction_by_1(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        ast_node_simplify_multipliaction_by_1(child);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '*') {
        return false;
    }

    AstNode *new_multiplication_node = create_new_node(MathOperatorToken, "*"); 
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];

        if (child->type != MathNumberToken) {
            append_child_node(
                new_multiplication_node, deep_clone_node(child)
            );
            free_ast(child);
            continue;
        }
        if (child->value != 1) {
            append_child_node(
                new_multiplication_node, deep_clone_node(child)
            );
            free_ast(child);
            continue;
        }
        free_ast(child);
    }

    free(node->children_ptrs);
    free(node->name);
    node->children_ptrs = new_multiplication_node->children_ptrs;
    node->child_count = new_multiplication_node->child_count;
    node->name = new_multiplication_node->name;

    free(new_multiplication_node);

    if (node->child_count == 0) {
        node->type = MathNumberToken;
        node->name = "1";
        node->value = 1;
        node->children_ptrs = NULL;

    } else if (node->child_count == 1) {
        AstNode *child = node->children_ptrs[0];
        free(node->children_ptrs);
        free(node->name);

        node->type = child->type;
        node->children_ptrs = child->children_ptrs;
        node->name = child->name;
        node->child_count = child->child_count;
        node->value = child->value;

        free(child);
    }

    return true;
}
