#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>


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


    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child_node_i = node->children_ptrs[i];
        size_t best_node_index = i;
        char *i_node_string = ast_node_to_string(child_node_i);
        int smallest_difference = 0;
        for(size_t j = i + 1; j < node->child_count; j++) {
            AstNode *child_node_j = node->children_ptrs[j];
            char *j_node_string = ast_node_to_string(child_node_j);
            int difference = strcmp(i_node_string, j_node_string);
            
            if (difference > smallest_difference) {
                smallest_difference = difference;
                best_node_index = j;
            }
            free(j_node_string);
        }
        free(i_node_string);

        node->children_ptrs[i] = node->children_ptrs[best_node_index];
        node->children_ptrs[best_node_index] = child_node_i;

    }
}
