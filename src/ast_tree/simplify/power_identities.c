#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>


bool ast_node_simplify_power_identities(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_power_identities(node->children_ptrs[i]);
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

    AstNode *first_thing = node->children_ptrs[0];
    AstNode *exponent = node->children_ptrs[1];

    if (exponent->type != MathNumberToken) {
        return false;
    }

    if (exponent->value == 1) {
        free_ast(exponent);
        replace_node_with_another(node, first_thing);
        return true;
    } else if (exponent->value == 0) {
        free_ast(exponent);
        free_ast(first_thing);
        replace_node_with_another(node, create_number_node(1));
        return true;
    }
}
