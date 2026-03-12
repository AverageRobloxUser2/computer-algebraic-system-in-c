#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

int find_gcd_euclids_algorithm(int a, int b) {
    // shout out my homie euclid
    if (b == 0) {
        return a;
    }
    if (a == b) {
        return a;
    }

    // euclids algorithm need to make sure we have max and min values
    int value_max = a;
    int value_min = b;

    if (a < b) {
        value_max = b;
        value_min = a;
    }

    return find_gcd_euclids_algorithm(value_min, value_max % value_max);
}

bool ast_node_simplify_fractions_by_gcd(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_fractions_by_gcd(node->children_ptrs[i]);
    }

    if (!ast_node_matches_requirements(node, MathOperatorToken, "*")) {
        return false;
    }

    AstNode *numerator_node = node->children_ptrs[0];
    if (numerator_node->type != MathNumberToken) {
        return false;
    }

    AstNode *denumerator_node = node->children_ptrs[1];

    if (!ast_node_matches_requirements(denumerator_node, MathOperatorToken, "^")) {
        return false;
    }


    AstNode *power_base = denumerator_node->children_ptrs[0];
    AstNode *power_exponent = denumerator_node->children_ptrs[1];

    if (!ast_node_matches_requirements(power_exponent, MathUnaryOperatorToken, "-")) {
        return false;
    }

    AstNode *power_exponent_number_value = power_exponent->children_ptrs[0];

    if (power_exponent_number_value->type != MathNumberToken) {
        return false;
    }
    // we have selected gone here if we have structure similar to this
    // 9*18^(-1) this is the structure of child_addend
    // 9 is numerator_node
    // 18 is power_base
    // -1 is power_exponent
    // 1 is power_exponent_number_value

    if (power_exponent_number_value->value != 1) {
        return false;
    }

    double int_part_not_used;
    if (modf(power_base->value, &int_part_not_used) != 0) {
        return false;
    }

    if (modf(numerator_node->value, &int_part_not_used) != 0) {
        return false;
    }

    // we test if both numerator_node and power_node are whole numbers

    int gcd = find_gcd_euclids_algorithm(numerator_node->value, power_base->value);

    if (gcd == 1) {
        return false;
    }

    numerator_node->value /= (double)gcd;
    power_base->value /= (double)gcd;

    ast_free_children(numerator_node);
    ast_shallow_free(numerator_node);

    replace_node_with_another(
        numerator_node, 
        create_number_node(numerator_node->value)
    );

    replace_node_with_another(
        power_base, 
        create_number_node(power_base->value)
    );

    return true;
}
