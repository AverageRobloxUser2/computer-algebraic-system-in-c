#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


AstNode *create_multiplication_node(AstNode *start_node, double count) {
    if (count == 1) {
        return deep_clone_node(start_node);
    } else if (count == 0) {
        return create_number_node(0);
    }
    AstNode *power_node = create_new_node(MathOperatorToken, "*");

    append_child_node(
        power_node, 
        create_number_node(fabs(count))
    );
    append_child_node(power_node, deep_clone_node(start_node));

    if (count < 0) {
        AstNode *negate_node = create_new_node(MathUnaryOperatorToken, "-");
        append_child_node(negate_node, power_node);
        return negate_node;
    }

    return power_node;
}

void concat_similar_nodes(AstNode *parent, AstNode *node) {
    // we deep clone so when we remove children its not freed
    node = deep_clone_node(node);

    double count = 0;
    for(size_t i = 0; i < parent->child_count; i++) {
        AstNode *child_node = parent->children_ptrs[i];

        if (ast_node_is_same_node(child_node, node)) {
            count++;
            remove_and_free_child_at_index(parent, i);
            i--;
            continue;
        }
        // unary operator and inside operator is same node
        // example node "a" and node "u_-(a)"
        if (child_node->type == MathUnaryOperatorToken && ast_node_is_same_node(
                    child_node->children_ptrs[0],
                    node
        )) {
            count --;
            remove_and_free_child_at_index(parent, i);
            i--;
            continue;
        }
    }

    append_child_node(
        parent,
        create_multiplication_node(node, count)
    );
    free_ast(node);
}

bool ast_node_simplify_addition_convert_to_multiplication(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_addition_convert_to_multiplication(
            node->children_ptrs[i]
        );
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '+') {
        return false;
    }

    if (node->child_count < 2) {
        return false;
    }

    for(size_t i = 0; i < node->child_count; i++) {
        concat_similar_nodes(node, *node->children_ptrs);
    }

    if (node->child_count == 1) {
        replace_node_with_another(
            node,
            node->children_ptrs[0]
        );
    }

    ast_node_concat_operators(node);

    return true;
}
