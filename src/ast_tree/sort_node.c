#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int compare_two_nodes_for_sorting(const void *node_a_ptr, const void *node_b_ptr) {
    AstNode *node_a = * (AstNode**)node_a_ptr;
    AstNode *node_b = * (AstNode**)node_b_ptr;
    // first numbers,
    // second variables,
    // then equations
    // returns true if node_a should come first
    if (node_a->type == MathNumberToken && node_b->type != MathNumberToken) {
        return -1;
    } else if (node_b->type == MathNumberToken && node_a->type != MathNumberToken) {
        return 1;
    } else if (node_a->type == MathVariableToken && node_b->type != MathVariableToken){
        return -1;
    } else if (node_b->type == MathVariableToken && node_a->type != MathVariableToken){
        return 1;
    } else if (node_a->type != MathUnaryOperatorToken && node_b->type == MathUnaryOperatorToken) {
        return -1;
    } else if (node_b->type != MathUnaryOperatorToken && node_a->type == MathUnaryOperatorToken) {
        return 1;
    }

    int difference = strcmp(node_a->name, node_b->name);

    if (difference < 0) {
        return -1;
    }

    return 1;
}

void sort_node(AstNode *node) {
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child_node = node->children_ptrs[i];
        sort_node(child_node);
    }

    if (node->type != MathOperatorToken) {
        return;
    }

    if (*node->name != '*') {
        if (*node->name != '+') {
            return;
        }
    }

    if (node->child_count > 10) {
        qsort(
                node->children_ptrs,
                node->child_count,
                sizeof(AstNode*),
                compare_two_nodes_for_sorting
        );
    } else {
        for(size_t i = 0; i < node->child_count-1; i++) {
            AstNode *child_a = node->children_ptrs[i];
            for(size_t j = i + 1; j < node->child_count; j++) {
                AstNode *child_b = node->children_ptrs[j];

                if (compare_two_nodes_for_sorting(&child_a, &child_b) < 0) {
                    continue;
                }

                node->children_ptrs[i] = child_b;
                node->children_ptrs[j] = child_a;

                child_a = node->children_ptrs[i];
            }
        }
    }
}
