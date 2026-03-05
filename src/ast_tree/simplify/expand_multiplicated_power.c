#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdlib.h>

bool ast_node_expand_multiplicated_power(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_expand_multiplicated_power(node->children_ptrs[i]);
    }


    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '^') {
        return false;
    }

    AstNode *base_node = node->children_ptrs[0];

    if (base_node->type != MathOperatorToken) {
        return false;
    }

    if (*base_node->name != '*') {
        return false;
    }

    AstNode *result_node = create_new_node(MathOperatorToken, "*");

    for(size_t i = 0; i < base_node->child_count; i++) {
        AstNode *power_node = create_new_node(MathOperatorToken, "^");
        AstNode *descendand_base = base_node->children_ptrs[i];
        append_child_node(power_node, deep_clone_node(descendand_base));

        for(size_t j = 1; j < node->child_count; j++) {
            AstNode *exponent_node = node->children_ptrs[j];
            append_child_node(power_node, deep_clone_node(exponent_node));
        }

        append_child_node(result_node, power_node);
    }

    for(size_t i = 0; i < node->child_count; i++) {
        free_ast(node->children_ptrs[i]);
    }

    replace_node_with_another(node, result_node);

    ast_node_concat_operators(node);

    return true;
}
