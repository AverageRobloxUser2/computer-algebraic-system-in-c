#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdlib.h>


bool ast_node_expand_negated_addition(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_expand_negated_addition(node->children_ptrs[i]);
    }

    if (!ast_node_matches_requirements(node, MathUnaryOperatorToken, "-")) {
        return false;
    }

    // we always have 1 child for unary
    AstNode *addition_node = node->children_ptrs[0];
    if (!ast_node_matches_requirements(addition_node, MathOperatorToken, "+")) {
        return false;
    }

    AstNode *result_node = create_new_node(MathOperatorToken, "+");

    result_node->child_count = addition_node->child_count;
    result_node->children_ptrs = malloc(addition_node->child_count * sizeof(AstNode*));

    for(size_t i = 0; i < addition_node->child_count; i++) {
        AstNode *negate_node = create_new_node(MathUnaryOperatorToken, "-");
        append_child_node(negate_node, addition_node->children_ptrs[i]);
        result_node->children_ptrs[i] = negate_node;
    }

    ast_shallow_free(node);
    replace_node_with_another(node, result_node);
}
