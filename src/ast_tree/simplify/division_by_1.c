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

    bool is_negated = false;
    for(size_t i = 1; i < node->child_count; i++){
        AstNode *denominator = node->children_ptrs[i];
        if (denominator->type != MathNumberToken) {
            continue;
        }
        if (denominator->value == -1) {
            is_negated = !is_negated;
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

    if (is_negated) {
        AstNode *negate_node = create_new_node(MathUnaryOperatorToken, "-");
        append_child_node(negate_node, deep_clone_node(node));
        replace_node_with_another(node, negate_node);
    }


    return true;
}
