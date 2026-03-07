#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>


bool ast_node_expand_multipcation(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_expand_multipcation(node->children_ptrs[i]);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '*') {
        return false;
    }

    size_t first_factor_equation_node_index = 0;
    bool found_equation_child = false;

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *factor = node->children_ptrs[i];
        if (factor->type != MathOperatorToken) {
            continue;
        }

        if (*factor->name != '+') {
            continue;
        }

        first_factor_equation_node_index = i;
        found_equation_child = true;
        break;
    }

    if (!found_equation_child) {
        return false;
    }


    AstNode *result_addition_node = create_new_node(MathOperatorToken, "+");
    AstNode *factor_node = node->children_ptrs[first_factor_equation_node_index];

    for(size_t i = 0; i < factor_node->child_count; i++) {
        AstNode *inner_factor_node = factor_node->children_ptrs[i];
        AstNode *addend_multiplication = create_new_node(MathOperatorToken, "*");
        append_child_node(
            addend_multiplication, 
            deep_clone_node(inner_factor_node)
        );
        for(size_t j = 0; j < node->child_count; j++) {
            if (j == first_factor_equation_node_index) {
                continue;
            }
            append_child_node(
                addend_multiplication, 
                deep_clone_node(node->children_ptrs[j])
            );
        }
        append_child_node(result_addition_node, addend_multiplication);
    }

    ast_free_children(node);
    replace_node_with_another(node, result_addition_node);
    ast_node_concat_operators(node);
    ast_node_expand_multipcation(node);
}
