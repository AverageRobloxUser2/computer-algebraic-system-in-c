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

    bool is_negated = false;

    for(size_t i = 0; i < node->child_count; i++){
        AstNode *factor_node = node->children_ptrs[i];

        if (factor_node->type == MathUnaryOperatorToken && *factor_node->name == '-') {
            is_negated = !is_negated;
        }

        if (factor_node->type != MathNumberToken) {
            continue;
        }


        if ((factor_node->value * factor_node->value) != 1) {
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
    } else if (node->child_count == 0) {
        replace_node_with_another(
            node,
            create_number_node(1)
        );
    }

    if (is_negated) {
        AstNode *negate_node = create_new_node(MathUnaryOperatorToken, "-");
        append_child_node(negate_node, deep_clone_node(node));
        replace_node_with_another(node, negate_node);
    }

    return true;
}
