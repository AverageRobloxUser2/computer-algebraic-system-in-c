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


void concat_power_nodes(AstNode *parent, AstNode *node) {
    // current algorithm
    // we keep a value number_exponent
    // itterate over all children and compare them with node
    // if they match then remove the child and increment number_exponent by 1
    // if they dont match we do extra checks
    //      1. check if its a power node. ie b*b^2
    //      2. if power node then we check the base of the power and compare it with node
    //          if they match proceed
    //      3. 
    // we deep clone so when we remove children its not freed
    node = deep_clone_node(node);

    double number_exponent = 0;
    AstNode *exponent_holder = create_new_node(MathOperatorToken, "+");
    for(size_t i = 0; i < parent->child_count; i++) {
        AstNode *child_node = parent->children_ptrs[i];

        if (ast_node_is_same_node(child_node, node)) {
            remove_and_free_child_at_index(parent, i);
            number_exponent++;
            i--;
            continue;
        }
        // unary operator and inside operator is same node
        // example node "a" and node "u_-(a)"
        if (child_node->type == MathOperatorToken) {
            if (*child_node->name == '^' && ast_node_is_same_node(
                        child_node->children_ptrs[0],
                        node
                )) {

                if (child_node->child_count == 2) {
                    append_child_node(
                        exponent_holder, 
                        deep_clone_node(child_node->children_ptrs[1])
                    );
                    remove_and_free_child_at_index(parent, i);
                    i--;
                }

                continue;
            }
        }
    }

    if (number_exponent == 1 && exponent_holder->child_count == 0) {
        append_child_node(
            parent,
            node
        );
        free_ast(exponent_holder);
        return;
    } 

    append_child_node(exponent_holder, create_number_node(number_exponent));

    AstNode *power_node = create_new_node(MathOperatorToken, "^");
    append_child_node(power_node, node);

    if (exponent_holder->child_count == 1) {
        append_child_node(
            power_node,
            create_number_node(number_exponent)
        );
        free_ast(exponent_holder);
    } else {
        append_child_node(power_node, exponent_holder);
    }

    append_child_node(
        parent,
        power_node
    );
}

bool ast_node_simplify_multiplication_convert_to_power(AstNode *node) {
    // return false;
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

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *current_node = node->children_ptrs[0];
        if (current_node->type == MathOperatorToken && *current_node->name == '^') {
            concat_power_nodes(node, current_node->children_ptrs[0]);
            continue;
        }
        concat_power_nodes(node, current_node);
    }

    if (node->child_count == 1) {
        append_child_node(
            node,
            create_number_node(1)
        );
    }

    ast_node_concat_operators(node);

    return true;
}
