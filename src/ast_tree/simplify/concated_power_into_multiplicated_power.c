#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>


bool ast_node_concated_power_into_multiplication(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_concated_power_into_multiplication(node->children_ptrs[i]);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '^') {
        return false;
    }

    if (node->child_count != 2) {
        return false;
    }

    AstNode *exponent_base = node->children_ptrs[0];

    if (exponent_base->type != MathOperatorToken) {
        return false;
    }

    if (*exponent_base->name != '^') {
        return false;
    }

    AstNode *factor_holder = create_new_node(MathOperatorToken, "*");

    append_child_node(
        factor_holder,
        deep_clone_node(exponent_base->children_ptrs[1])
    );
    append_child_node(
        factor_holder,
        deep_clone_node(node->children_ptrs[1])
    );

    remove_and_free_child_at_index(node, 1);
    remove_and_free_child_at_index(exponent_base, 1);

    append_child_node(node, factor_holder);

    return true;
}
