#include "ast_tree.h"
#include "math_equation.h"
#include <math.h>
#include <stddef.h>

void simplify_unary_negate(AstNode *node) {
    AstNode *child = node->children_ptrs[0];

    if (child->type != MathUnaryOperatorToken) {
        return;
    }

    if (*child->name != '-') {
        return;
    }

    AstNode *real_value = child->children_ptrs[0];
    AstNode *value_clone = deep_clone_node(real_value);
    remove_and_free_child(node, child);

    replace_node_with_another(node, value_clone);
}

bool ast_node_simplify_double_unary(AstNode *node) {

    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_double_unary(node->children_ptrs[i]);
    }

    if (node->type != MathUnaryOperatorToken) {
        return false;
    }

    switch (*node->name) {
        case '-':
            simplify_unary_negate(node);
            break;
        case '+':
            replace_node_with_another(node, node->children_ptrs[0]);
            break;
    }
}

void expand_addition(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i]; 
        if (child->type != MathUnaryOperatorToken) {
            continue;
        }

        if (*child->name != '-') {
            continue;
        }

        AstNode *negated_descendant = child->children_ptrs[0];

        if (negated_descendant->type != MathOperatorToken) {
            continue;
        }

        if (*negated_descendant->name != '+') {
            continue;
        }

        for(size_t j = 0; j < negated_descendant->child_count; j++) {
            AstNode *unary_negate = create_new_node(MathUnaryOperatorToken, "-");
            append_child_node(
                    unary_negate, 
                    deep_clone_node(negated_descendant->children_ptrs[j])
            );
            append_child_node(node, unary_negate);
        }
        remove_and_free_child_at_index(node, i);
        i--;
    }
}

void expand_multiplication(AstNode *node) {
    bool negated = false;

    for(size_t i = 0;i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        if (child->type != MathUnaryOperatorToken) {
            continue;
        }

        if (*child->name != '-') {
            continue;
        }
        negated = !negated;
        replace_node_with_another(child, child->children_ptrs[0]);
    }

    if (!negated) {
        return;
    }

    AstNode *negation_node = create_new_node(MathUnaryOperatorToken, "-");
    append_child_node(negation_node, node->children_ptrs[0]);
    node->children_ptrs[0] = negation_node;
}

bool ast_node_expand_unary(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_expand_unary(node->children_ptrs[i]);
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name == '+') {
        expand_addition(node);
        return true;
    }

    if (*node->name == '*') {
        expand_multiplication(node);
        return true;
    }
}
