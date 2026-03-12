#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stdio.h>


void constant_fold_addition(AstNode *node) {
    double total_sum = 0;
    // printf("CONSTANT FOLD ADDITION:");
    // print_ast_as_string(node);

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *addend = node->children_ptrs[i];
        if (addend->type == MathUnaryOperatorToken && *addend->name == '-') {
            AstNode *value = addend->children_ptrs[0];
            if (value->type == MathNumberToken) {
                total_sum -= value->value;
                remove_and_free_child_at_index(node, i);
                i--;
            }
            continue;
        } else if (addend->type == MathNumberToken) {
            total_sum += addend->value;
            remove_and_free_child_at_index(node, i);
            i--;
        }
    }

    if (total_sum != 0) {
        append_child_node(node, create_number_node(total_sum));
    }
    if (node->child_count == 0) {
        replace_node_with_another(node, create_number_node(0));
    }
    if (node->child_count == 1) {
        replace_node_with_another(node, node->children_ptrs[0]);
    }
}

void constant_fold_multiplication(AstNode *node) {
    double total_result = 1;

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *factor_node = node->children_ptrs[i];
        if (factor_node->type == MathUnaryOperatorToken 
                && *factor_node->name == '-') {
            AstNode *value = factor_node->children_ptrs[0];
            if (value->type == MathNumberToken) {
                total_result *= -value->value;
                remove_and_free_child_at_index(node, i);
                i--;
            }
            continue;
        } else if (factor_node->type == MathNumberToken) {
            total_result *= factor_node->value;
            remove_and_free_child_at_index(node, i);
            i--;
        }
    }

    if (total_result != 1) {
        append_child_node(node, create_number_node(total_result));
    }
    if (node->child_count == 0) {
        replace_node_with_another(node, create_number_node(1));
    }
    if (node->child_count == 1) {
        replace_node_with_another(node, node->children_ptrs[0]);
    }
}

void constant_fold_power(AstNode *node) {

    if (!ast_node_only_contains_numbers(node)) {
        return;
    }

    float result = node->children_ptrs[0]->value;
    for(size_t i = 1; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        double int_part = 0;

        if (modf(child->value, &int_part) != 0) {
            return;
        }

        result = powf(result, child->value);
    }


    ast_free_children(node);
    replace_node_with_another(node, create_number_node(result));
}

bool ast_node_constant_fold(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_constant_fold(node->children_ptrs[i]);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    switch (*node->name) {
        case '+':
            constant_fold_addition(node);
            break;
        case '*':
            constant_fold_multiplication(node);
            break;
        case '^':
            constant_fold_power(node);
            break;
    }
}
