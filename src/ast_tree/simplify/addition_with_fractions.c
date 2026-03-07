#include "ast_tree.h"
#include "math_equation.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

bool is_fraction(AstNode *node) {
    if (!ast_node_matches_requirements(node, MathOperatorToken, "*")) {
        return false;
    }

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        if (!ast_node_matches_requirements(child, MathOperatorToken, "^")) {
            continue;
        }

        AstNode *exponent = child->children_ptrs[1];

        if (!ast_node_matches_requirements(exponent, MathUnaryOperatorToken, "-")) {
            continue;
        }

        return true;
    }

    return false;
}

AstNode *find_common_denominator(AstNode *fractions_only) {
    AstNode *common_denominator = create_new_node(MathOperatorToken, "*");

    for(size_t i = 0; i < fractions_only->child_count; i++) {
        AstNode *fraction = fractions_only->children_ptrs[i];
        for(size_t j = 0; j < fraction->child_count; j++) {
            AstNode *fraction_factor = fraction->children_ptrs[j];
            bool is_power_node = ast_node_matches_requirements(
                fraction_factor,
                MathOperatorToken,
                "^" 
            );

            if (!is_power_node) {
                continue;
            }

            AstNode *exponent = fraction_factor->children_ptrs[1];

            bool is_negate_node = ast_node_matches_requirements(
                exponent,
                MathUnaryOperatorToken,
                "-"
            );

            if (!is_negate_node) {
                continue;
            }

            AstNode *power_node = create_new_node(MathOperatorToken, "^");

            append_child_node(
                power_node,
                deep_clone_node(fraction_factor->children_ptrs[0])
            );
            append_child_node(
                power_node,
                deep_clone_node(exponent->children_ptrs[0])
            );
            // append_child_node(
            //     common_denominator, 
            //     deep_clone_node(fraction_factor->children_ptrs[0])
            // );
            append_child_node(common_denominator, power_node);
        }
    }

    return common_denominator;
}

AstNode *compute_fraction_numerator(AstNode *fractions_only, AstNode *common_denominator) {

    AstNode *numerator = create_new_node(MathOperatorToken, "+");

    for(size_t i = 0; i < fractions_only->child_count; i++) {
        AstNode *fraction = fractions_only->children_ptrs[i];

        AstNode *result = create_new_node(MathOperatorToken, "*");
        AstNode *specific_denominator = deep_clone_node(common_denominator);
        for(size_t j = 0; j < fraction->child_count; j++) {
            AstNode *fraction_factor = fraction->children_ptrs[j];
            bool is_power_node = ast_node_matches_requirements(
                fraction_factor,
                MathOperatorToken,
                "^" 
            );

            if (!is_power_node) {
                append_child_node(result, deep_clone_node(fraction_factor));
                continue;
            }

            AstNode *exponent = fraction_factor->children_ptrs[1];

            bool is_negate_node = ast_node_matches_requirements(
                exponent,
                MathUnaryOperatorToken,
                "-"
            );

            if (!is_negate_node) {
                append_child_node(result, deep_clone_node(fraction_factor));
                continue;
            }

            AstNode *power_node = create_new_node(MathOperatorToken, "^");

            append_child_node(
                power_node,
                fraction_factor->children_ptrs[0]
            );
            append_child_node(
                power_node,
                exponent->children_ptrs[0]
            );
            remove_and_free_child(
                specific_denominator, 
                power_node
            );

            ast_shallow_free(power_node);
            free(power_node);
        }

        if (specific_denominator->child_count == 1) {
            replace_node_with_another(
                specific_denominator,
                specific_denominator->children_ptrs[0]
            );
        } else if (specific_denominator->child_count == 0) {
            replace_node_with_another(
                specific_denominator,
                create_number_node(1)
            );
        }

        append_child_node(result, specific_denominator);

        ast_node_simplify_multiplication_convert_to_power(result);
        append_child_node(numerator, result);
    }
    return numerator;
}

bool ast_node_simplify_addition_with_fractions(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_addition_with_fractions(node->children_ptrs[i]);
    }

    if (!ast_node_matches_requirements(node, MathOperatorToken, "+")) {
        return false;
    }

    AstNode *fractions_only = create_new_node(MathOperatorToken, "+");

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child_addend = node->children_ptrs[i];


        if (child_addend->type == MathOperatorToken && *child_addend->name == '^') {
            AstNode *temp_child_holder = create_new_node(MathOperatorToken, "*");
            append_child_node(temp_child_holder, create_number_node(1));
            append_child_node(temp_child_holder, child_addend);

            node->children_ptrs[i] = temp_child_holder;
            i--;
            continue;
        }

        if (!is_fraction(child_addend)) {
            continue;
        }

        append_child_node(fractions_only, deep_clone_node(child_addend));
    }

    if (fractions_only->child_count < 2) {
        free_ast(fractions_only);
        return false;
    }
    
    // printf("Fractions only has %zu children\n", fractions_only->child_count);

    AstNode *common_denominator = find_common_denominator(fractions_only);

    for(int i = 0; i < (int)common_denominator->child_count; i++) {
        AstNode *child = common_denominator->children_ptrs[i];
        if (find_child_index(common_denominator, child) != i) {
            free_ast(common_denominator);
            free_ast(fractions_only);
            return false;
        }
    }

    // ast_node_simplify_power_identities(common_denominator);
    // ast_node_simplify_multipliaction_by_1(common_denominator);

    // printf("common_denominator:");
    // print_ast_as_string(common_denominator);

    AstNode *numerator = compute_fraction_numerator(fractions_only, common_denominator);
    // ast_node_simplify_power_identities(numerator);
    // ast_node_simplify_multipliaction_by_1(numerator);
    //
    // printf("numerator:");
    // print_ast_as_string(numerator);

    AstNode *result = create_new_node(MathOperatorToken, "/"); 

    append_child_node(result, numerator);
    append_child_node(result, common_denominator);

    ast_node_division_into_multiplication(result);


    for(size_t i = 0; i < fractions_only->child_count; i++) {
        remove_and_free_child(node, fractions_only->children_ptrs[i]);
    }

    ast_node_constant_fold(result);
    ast_node_simplify_power_identities(result);
    ast_node_simplify_multipliaction_by_1(result);
    ast_node_constant_fold(result);

    free_ast(fractions_only);


    append_child_node(node, result);

    if (node->child_count == 1) {
        // printf("replacing\n");
        replace_node_with_another(node, node->children_ptrs[0]);
    }


    // printf("done!\n");
}
