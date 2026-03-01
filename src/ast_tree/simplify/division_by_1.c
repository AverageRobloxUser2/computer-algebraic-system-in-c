#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

bool ast_node_simplify_division_by_1(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        ast_node_simplify_division_by_1(child);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '/') {
        return false;
    }

    for(size_t i = 1; i < node->child_count; i++){
        AstNode *denominator = node->children_ptrs[i];
        if (denominator->type != MathNumberToken) {
            continue;
        }
        if (denominator->value != 1) {
            continue;
        }
        remove_and_free_child_at_index(node, i);
        i--;
    }

    if (node->child_count == 1) {
        replace_node_with_another(
            node,
            node->children_ptrs[0]
        );
    }

    return true;
}
