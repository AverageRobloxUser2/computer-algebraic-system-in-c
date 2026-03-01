#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

bool check_is_multiplication(AstNode *node) {
    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name == '*') {
        return true;
    }

    return false;
}

size_t count_common_factors(AstNode *addition_node, AstNode *factor_node) {
    size_t factor_found_count = 0;

    for (size_t i = 0; i < addition_node->child_count; i++) {
        AstNode *addend_node = addition_node->children_ptrs[i];
        int child_index = find_child_index(addend_node, factor_node); 

        if (child_index == -1) {
            continue;
        }
        factor_found_count++;
    }
    return factor_found_count;
}

void extract_factors_from_addition_node(AstNode *addition_node, AstNode *factor_node) {
    // createws a new node that holds the simplified addition node 
    // for example ab+bd and factor b would converted into
    // b(a+d)
    // algorithm:
    // itterate over children of addition_node
    // if child has factor node as its child then we remove the child from addition node
    // and append it to addend_holder_node while also removing the factor node
    // from the child's children nodes
    // then append result node to addition_node
    AstNode *factor_clone = deep_clone_node(factor_node);
    AstNode *result_node = create_new_node(MathOperatorToken, "+");
    AstNode *multiplication_node = create_new_node(MathOperatorToken, "*");
    AstNode *addend_holder_node = create_new_node(MathOperatorToken, "+");

    append_child_node(result_node, multiplication_node);
    append_child_node(multiplication_node, factor_clone);
    append_child_node(multiplication_node, addend_holder_node);

    for(size_t i = 0; i < addition_node->child_count; i++) {
        AstNode *addend_node = addition_node->children_ptrs[i];
        addend_node = deep_clone_node(addend_node);
        int factor_index = find_child_index(addend_node, factor_clone);

        if (factor_index == -1) {
            append_child_node(result_node, addend_node);
            continue;
        }

        remove_and_free_child(addition_node, addend_node);
        // since we remove 1 child we move "i" one back too
        i--;

        remove_and_free_child(addend_node, factor_clone);
        if (addend_node->child_count == 1) {
            append_child_node(
                addend_holder_node, 
                deep_clone_node(addend_node->children_ptrs[0])
            );
            free_ast(addend_node);
        } else {
            append_child_node(addend_holder_node, addend_node);
        }
    }

    ast_node_simplify_same_multiplicator_addition(addend_holder_node);

    if (result_node->child_count == 1) {
        replace_node_with_another(
                addition_node, 
                deep_clone_node(result_node->children_ptrs[0])
        );
        free_ast(result_node);
    } else {
        replace_node_with_another(addition_node, result_node);
    }
}

bool ast_node_simplify_same_multiplicator_addition(AstNode *node) {
    // quite complicated algorithm so ill explain
    // first sepearte multiplication nodes from addition node
    // we will append all the multiplication nodes into a sepearte addition node
    // at the end of the proces we will merge the addition nodes
    // we now itterate over all children of the addition node
    // we then store information about the factors: parent, index,  
    // and how many other nodes have the same factor.
    // in this way we can find the most common factor
    // we do this instead of taking factors of first multiplication node 
    // so we simplify better
    // for example equation "abc + aef + jen + 2e"
    // most shared is "e" so we will simplify with it instead of "a"
    // also since now we hold the three information points
    // now imagine in my previous equation instead of "e" we had "xy"
    // this would allow for simplification of two factors
    // how do we handle that?
    // well just recursivly call this function on the result of previous
    // simplification :3
    // then result would be x(y(...)) which is equal to (xy)(...)

    if (node->type != MathOperatorToken) {
        return false;
    }

    if (*node->name != '+') {
        return false;
    }

    AstNode *addition_node = create_new_node(MathOperatorToken, "+");
    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *addend = node->children_ptrs[i];
        if(!check_is_multiplication(addend)) {
            continue;
        }

        AstNode *addend_clone = deep_clone_node(addend);
        remove_and_free_child(node, addend);
        i--;
        append_child_node(addition_node, addend_clone);
    }

    char *most_common_factor_as_string = NULL;
    AstNode *most_common_factor = NULL;
    size_t most_common_factor_found_count = 0;

    for(size_t i = 0; i < addition_node->child_count; i++) {
        AstNode *addend_node = addition_node->children_ptrs[i];
        for(size_t j = 0; j < addend_node->child_count; j++) {
            AstNode *factor_node = addend_node->children_ptrs[j];
            char *factor_as_string = ast_node_to_string(factor_node);

            // check if we already processed this factor
            if (most_common_factor_as_string != NULL) {
                if (strcmp(factor_as_string, most_common_factor_as_string) == 0) {
                    free(factor_as_string);
                    continue;
                }
            }

            size_t factor_appearence_count = count_common_factors(
                addition_node,
                factor_node
            );

            if (factor_appearence_count > most_common_factor_found_count) {
                most_common_factor_found_count = factor_appearence_count;
                free(most_common_factor_as_string);
                most_common_factor_as_string = factor_as_string;
                most_common_factor = factor_node;
            } else {
                free(factor_as_string);
            }
        }
    }
    free(most_common_factor_as_string);

    bool replaced_anything = most_common_factor_found_count > 1;
    if (most_common_factor_found_count > 1) {
        extract_factors_from_addition_node(
                addition_node,
                most_common_factor
        );
    }

    if (node->child_count == 0) {
        replace_node_with_another(node, addition_node);
    } else {
        append_child_node(node, addition_node);
    }

    ast_node_concat_operators(node);

    return replaced_anything;
}
