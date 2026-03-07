#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stddef.h>


bool ast_node_simplify_power_identities(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_power_identities(node->children_ptrs[i]);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '^') {
        return false;
    }

    for(size_t i = 1; i < node->child_count; i++) {
        AstNode *exponent = node->children_ptrs[i];
        double number_value = 0;

        if (exponent->type != MathNumberToken && exponent->type != MathUnaryOperatorToken) {
            continue;
        }

        if (exponent->type == MathUnaryOperatorToken) {
            if (*exponent->name == '-' &&  ast_node_only_contains_numbers(exponent)) {
                number_value = -exponent->children_ptrs[0]->value;
            } else {
                continue;
            }
        } else {
            number_value = exponent->value;
        }

        if (number_value == 0) {
            ast_free_children(node);
            replace_node_with_another(node, create_number_node(1));
        } else if (number_value == 1) {
            remove_and_free_child_at_index(node, i);
            i--;
        }
    }


    if (node->child_count == 1) {
        replace_node_with_another(node, node->children_ptrs[0]);
    }

    return true;
}
