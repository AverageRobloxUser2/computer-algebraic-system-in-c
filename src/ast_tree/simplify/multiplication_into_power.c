#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


AstNode *create_power_node(AstNode *start_node, size_t succesive_nodes) {
    if (succesive_nodes < 2) {
        return deep_clone_node(start_node);
    }
    AstNode *power_node = create_new_node(MathOperatorToken, "^");

    append_child_node(power_node, deep_clone_node(start_node));

    char value_node_name[32];
    sprintf(value_node_name, "%ld", succesive_nodes);

    append_child_node(
        power_node, 
        create_new_node(MathNumberToken, value_node_name)
    );

    return power_node;
}
bool ast_node_simplify_multiplication_convert_to_power(AstNode *node) {
    // algorithm explanation
    // first recursivly simplify all children
    // we have a variable called "succesive_nodes". it stores the amount
    // of times a node appears in success
    // for example in equation
    // "a * a * b"
    // succesive_nodes for a will be 2, which we will then create
    // a equation "a^2 * b"
    for(size_t i = 0; i < node->child_count; i++) {
        ast_node_simplify_multiplication_convert_to_power(
            node->children_ptrs[i]
        );
    }

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '*') {
        return false;
    }

    if (node->child_count < 2) {
        return false;
    }


    int succesive_nodes = 1;
    AstNode *start_node = *node->children_ptrs;
    char *start_str = ast_node_to_string(start_node);
    

    AstNode **old_children = node->children_ptrs+1;
    size_t old_child_count = node->child_count-1;

    node->child_count = 0;
    node->children_ptrs = NULL;

    bool changed = false;

    for(size_t i = 0; i < old_child_count; i++) {
        AstNode *current_node = old_children[i];

        char *current_str = ast_node_to_string(current_node);

        bool same = strcmp(current_str, start_str) == 0;

        if (!same) {
            append_child_node(
                    node,
                    create_power_node(start_node, succesive_nodes)
            );

            free(start_str);
            free_ast(start_node);


            if (succesive_nodes > 1) {
                changed = true;
            }

            start_node = current_node;
            start_str = current_str;
            succesive_nodes = 1;

            continue;
        }

        free(current_str);
        free_ast(current_node);

        succesive_nodes += 1;
    }

    append_child_node(
        node,
        create_power_node(start_node, succesive_nodes)
    );

    if (node->child_count == 1) {
        append_child_node(
            node,
            create_number_node(1)
        );
    }

    free(old_children - 1);

    free(start_str);
    free_ast(start_node);


    return changed;
}
