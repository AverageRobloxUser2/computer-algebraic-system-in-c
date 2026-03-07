#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stddef.h>


bool ast_node_conditionaly_expand_power(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_conditionaly_expand_power(node);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '^') {
        return false;
    }

    AstNode *base_node = node->children_ptrs[0];
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
    if (exponent_node->value > 3) {
        return false;
    }

    double int_part_of_exponent = 0;
    if (modf(exponent_value, &int_part_of_exponent) != 0) {
        return false;
    }

    if (base_node->child_count > 2) {
        return false;
    }

    AstNode *base_clone = deep_clone_node(base_node);
    
    // remove first and second child of our power node
    remove_and_free_child_at_index(node, 0);
    remove_and_free_child_at_index(node, 0);


    AstNode *result_node = create_new_node(MathOperatorToken, "*");

    for(size_t i = 0; i < int_part_of_exponent; i++) {
        append_child_node(result_node, deep_clone_node(base_clone));
    }
}
