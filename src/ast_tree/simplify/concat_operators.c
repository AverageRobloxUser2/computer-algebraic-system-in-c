#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool simplify_node(AstNode *node) {
    if (node->type == MathUnaryOperatorToken) {
        return false;
    }
    AstNode *child = NULL;
    size_t i;
    for(i = 0; i < node->child_count; i++){
        AstNode *current_child = node->children_ptrs[i];


        if (current_child->type != node->type) {
            continue;
        }

        if (strcmp(current_child->name, node->name) != 0) {
            continue;
        }
        child = current_child;
        break;
    }

    if (child == NULL) {
        return false;
    }

    AstNode **new_child_ptrs = calloc(
        child->child_count + node->child_count, 
        sizeof(AstNode*)
    );

    size_t child_node_child_ptrs_length = child->child_count * sizeof(AstNode*) ;
    size_t node_child_ptrs_length_before = i * sizeof(AstNode*);
    size_t node_child_ptrs_length_after = (node->child_count - i - 1) * sizeof(AstNode*);

    printf(
        "lenghts: %ld %ld %ld\n",
        node_child_ptrs_length_before,
        child_node_child_ptrs_length,
        node_child_ptrs_length_after
    );

    memcpy(
        new_child_ptrs,
        node->children_ptrs,
        node_child_ptrs_length_before
    );

    memcpy(
        new_child_ptrs + i,
        child->children_ptrs,
        child_node_child_ptrs_length
    );

    memcpy(
        new_child_ptrs + i + child->child_count,
        node->children_ptrs + i + 1,
        node_child_ptrs_length_after
    );

    node->child_count = node->child_count + child->child_count - 1;

    free(child->name);
    free(child->children_ptrs);
    free(child);
    free(node->children_ptrs);

    node->children_ptrs = new_child_ptrs;

    return true;
}

bool ast_node_concat_operators(AstNode *node) {
    if (node->child_count == 0) {
        return false;
    }

    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_concat_operators(node->children_ptrs[i]);
    }

    bool simplified = simplify_node(node);
    while (simplified) {
        simplified = simplify_node(node);
    }

    return simplified;
}
