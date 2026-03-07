#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stddef.h>


bool ast_node_conditionaly_expand_power(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_conditionaly_expand_power(node->children_ptrs[i]);
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

    AstNode *base_node = node->children_ptrs[0];

    if (base_node->type != MathOperatorToken) {
        return false;
    }

    if (base_node->child_count != 2) {
        return false;
    }

    AstNode *exponent_node = node->children_ptrs[1];

    double exponent_value = 0;

    if (exponent_node->type != MathNumberToken) {
        if (exponent_node->type == MathUnaryOperatorToken 
                && *exponent_node->name == '-'
                && ast_node_only_contains_numbers(exponent_node)) {
            exponent_value = -exponent_node->children_ptrs[0]->value;
        } else {
            return false;
        }
    } else {
        exponent_value = exponent_node->value; 
    }

    if (exponent_value < 0) {
        return false;
    }

    if (exponent_node->value > 3) {
        return false;
    }

    double int_part_of_exponent = 0;
    if (modf(exponent_value, &int_part_of_exponent) != 0) {
        return false;
    }

    AstNode *result_node = create_new_node(MathOperatorToken, "*");

    for(size_t i = 0; i < int_part_of_exponent; i++) {
        append_child_node(result_node, deep_clone_node(base_node));
    }

    ast_free_children(node);
    replace_node_with_another(node, result_node);
}
