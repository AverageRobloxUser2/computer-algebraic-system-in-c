#include "rules.h"
#include "ast_tree.h"
#include "math_equation.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    OrderDoesntMatter,
    KeepFirst,
    OrderMatters,
    WrongNodeType
} MathOrderOfEquation;


MathOrderOfEquation get_order(AstNode *node) {
    if (node->type != MathOperatorToken ) {
        return WrongNodeType;
    }

    switch (*node->name) {
        case '+':
        case '*':
            return OrderDoesntMatter;
        case '/':
        case '-':
            return KeepFirst;
        case '^':
            return OrderMatters;
        default:
            return WrongNodeType;
    }
}

AstNode *mutate_node(
    AstNode *node, 
    size_t child_i,
    bool reverse
) {
    MathOrderOfEquation order = get_order(node);
    AstNode *new_node = create_new_node(node->type, node->name);
    if (node->child_count <= 2) {
        if (reverse) {
            for(size_t i = node->child_count; i > 0 ; i--) {
                append_child_node(new_node, node->children_ptrs[i-1]);
            }
        } else {
            for(size_t i = 0; i < node->child_count; i++) {
                append_child_node(new_node, node->children_ptrs[i]);
            }
        }

        return new_node;
    }

    AstNode *child_holder = create_new_node(node->type, node->name);

    if (order == OrderDoesntMatter) {
        AstNode *selected_child = node->children_ptrs[child_i];
        if (!reverse) {
            append_child_node(new_node, selected_child);
            append_child_node(new_node, child_holder);
        } else {
            append_child_node(new_node, child_holder);
            append_child_node(new_node, selected_child);
        }
        for(size_t i = 0; i < node->child_count; i++) {
            if (i == child_i) {
                continue;
            }

            AstNode *child = node->children_ptrs[i];
            append_child_node(child_holder, child);
        }
    } else {
        child_i = 0;
        AstNode *selected_child = node->children_ptrs[child_i];
        append_child_node(new_node, selected_child);
        append_child_node(new_node, child_holder);

        for(size_t i = 0; i < node->child_count; i++) {
            if (i == child_i) {
                continue;
            }

            AstNode *child = node->children_ptrs[i];
            append_child_node(child_holder, child);
        }
    }
    return new_node;
}

bool check_if_can_create_variable_map(
        AstNode *node, 
        AstNode *rule
) {
    MathVariableMap *map = create_variable_map(
        node,rule
    );

    if (map == NULL) {
        return false;
    }
    printf("can crete map %p :3\n", (void*)map);
    
    free_variable_map(map);
    return true;
}

void print_depth(size_t depth) {
    for(size_t i = 0; i < depth; i++) {
        printf("\t");
    }
}

AstNode **find_first_mutable_child(AstNode **node_ptr) {
    AstNode *node = *node_ptr;
    if (node->child_count > 2) {
        return node_ptr;
    }
    for(size_t i = 0; i < node->child_count;i++) {
        AstNode **child_ptr = &node->children_ptrs[i];
        AstNode **found_mutable = find_first_mutable_child(child_ptr);
        if (found_mutable != NULL) {
            return found_mutable;
        }
    }

    return NULL;
}

MathVariableMap *compare_node_with_rule(
        AstNode *node,
        AstNode *rule,
        AstNode *starting_node,
        AstNode *starting_rule,
        size_t depth
) {
    MathVariableMap *map = create_variable_map(
        starting_node, 
        starting_rule
    );

    if (map != NULL) {
        print_depth(depth);
        printf("GREAT SUCCESS!\n");
        return map;
    }

    if(node->type != rule->type) {
        print_depth(depth);
        printf("wrong types\n");
        return NULL;
    }

    if (strcmp(node->name, rule->name) != 0) {
        print_depth(depth);
        printf("wrong names\n");
        return NULL;
    }

    AstNode** mutable_child_ptr = find_first_mutable_child(&node);

    if (mutable_child_ptr == NULL) {
        printf("no mutable child found\n");
        return NULL;
    }

    AstNode *mutable_child = *mutable_child_ptr;

    for(size_t j = 0; j < 2; j++) {
        for(size_t i = 0; i < mutable_child->child_count; i++) {
            AstNode *mutated_node = mutate_node(mutable_child, i, j==0);

            print_depth(depth);
            print_ast_as_string(mutated_node);

            *mutable_child_ptr = mutated_node;

            MathVariableMap *match = NULL;
            if (starting_node != mutable_child) {
                match = compare_node_with_rule(
                    node,
                    rule,
                    starting_node,
                    starting_rule,
                    depth+1
                );
            } else {
                match = compare_node_with_rule(
                    mutated_node,
                    rule,
                    mutated_node,
                    starting_rule,
                    depth
                );
            }

            *mutable_child_ptr = mutable_child;


            free(mutated_node->name);
            free(mutated_node->children_ptrs);
            free(mutated_node);

            if (match != NULL) {
                return match;
            }

        }
    }

    printf("just invalid\n");

    return NULL;
}

MathVariableMap *does_node_match_rule(AstNode *node, AstNode *rule_node) {

    if (node->type != rule_node->type) {
        // printf("nodes not equal based on node type\n");
        return NULL;
    }

    if (strcmp(node->name, rule_node->name) != 0) {
        // printf("nodes not equal based on node names\n");
        return NULL;
    }

    if (node->child_count < rule_node->child_count) {
        // printf("nodes not equal based on node child_count\n");
        return NULL;
    }

    return compare_node_with_rule(node, rule_node, node, rule_node, 0);
}

AstNode *create_node_form_rules(AstNode *rule, MathVariableMap *map) {
    AstNode *result = create_new_node(
        rule->type,
        rule->name
    );

    if (rule->type == MathVariableToken) {
        free_ast(result);
        result = variable_map_get_variable_value(map, rule->name);
        return deep_clone_node(result);
    }

    for(size_t i = 0; i < rule->child_count; i++){
        AstNode *child_rule = rule->children_ptrs[i];
        AstNode *to_append = NULL;
        if (child_rule->type == MathVariableToken) {
            to_append = deep_clone_node(
                variable_map_get_variable_value(map, child_rule->name)
            );
        } else if( child_rule->type == MathNumberToken) {
            to_append = deep_clone_node(child_rule);
        } else {
            to_append = create_node_form_rules(child_rule, map);
        }

        if (to_append == NULL) {
            printf("to_append child is NULL, it should not be");
            exit(1);
        }
        append_child_node(result, to_append);
    }

    return result;
}
