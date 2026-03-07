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
    bool negated = false;
    bool has_zero = false;

    for(size_t i = 0; i < node->child_count; i++){
        AstNode *factor_node = node->children_ptrs[i];

        if (factor_node->type == MathNumberToken) {
            if (factor_node->value == 0) {
                has_zero = true;
                break;
            }

            if ((factor_node->value * factor_node->value) != 1) {
                continue;
            }

            remove_and_free_child_at_index(node, i);
            i--;
        } else if (ast_node_matches_requirements(
                    factor_node, MathUnaryOperatorToken, "-"
                    )) {
            node->children_ptrs[i] = factor_node->children_ptrs[0];
            ast_shallow_free(factor_node);
            free(factor_node);
            negated = !negated;
        }
    }

    if (has_zero) {
        for(size_t i = 0; i < node->child_count; i++) {
            free_ast(node->children_ptrs[i]);
        }

        replace_node_with_another(
            node,
            create_number_node(0)
        );
        return true;
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

    if (negated) {
        AstNode *negate_op = create_new_node(MathUnaryOperatorToken, "-");
        append_child_node(negate_op, deep_clone_node(node));
        ast_free_children(node);
        replace_node_with_another(node, negate_op);
    }

    return true;
}
