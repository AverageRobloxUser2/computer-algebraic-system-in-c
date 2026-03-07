#include "ast_tree.h"
#include "math_equation.h"

#include <stdio.h>
#include <stdlib.h>

bool ast_node_subtraction_into_negated_addition(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child_node = node->children_ptrs[i];
        ast_node_subtraction_into_negated_addition(child_node);
    }

    if(node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '-') {
        return false;
    }

    AstNode **new_children = calloc(node->child_count, sizeof(AstNode*));

    new_children[0] = node->children_ptrs[0];

    for(size_t i = 1; i < node->child_count; i++) {
        AstNode *negate_node = create_new_node(
            MathUnaryOperatorToken,
            "-"
        );

        AstNode *child_node = node->children_ptrs[i];

        append_child_node(negate_node, child_node);
        new_children[i] = negate_node; 
    }

    free(node->children_ptrs);
    node->children_ptrs = new_children;
    *(node->name) = '+';

    return true;
}

bool ast_node_division_into_multiplication(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child_node = node->children_ptrs[i];
        ast_node_division_into_multiplication(child_node);
    }

    if(node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '/') {
        return false;
    }

    AstNode **new_children = calloc(node->child_count, sizeof(AstNode*));

    new_children[0] = node->children_ptrs[0];

    for(size_t i = 1; i < node->child_count; i++) {
        AstNode *power_node = create_new_node(MathOperatorToken, "^");

        AstNode *child_node = node->children_ptrs[i];

        append_child_node(power_node, child_node);
        append_child_node(power_node, create_number_node(-1));
        new_children[i] = power_node; 
    }

    free(node->children_ptrs);
    node->children_ptrs = new_children;
    node->name[0] = '*';

    return true;
}
