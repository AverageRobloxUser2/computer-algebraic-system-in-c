#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evaluator.h"
#include <limits.h>


void print_tree(ExpresionNode *node, int depth) {
    printf("%s\n", node_to_string(node));
    // // starting depth is 1
    // char *indent = calloc(sizeof(char), depth);
    //
    // for(int i = 0; i < depth - 1; i++) {
    //     indent[i] = '\t';
    // }
    // printf("%s", indent);
    // free(indent);
    // switch (node->type) {
    //     case TokenTypeVariable:
    //         printf("variable: %s", node->name);
    //         break;
    //     case TokenTypeNumber:
    //         printf("number: %s", node->name);
    //         break;
    //     case TokenTypeFunction:
    //         printf("function: %s", node->name);
    //         break;
    //     case TokenTypeOperator:
    //     case TokenTypeUnaryOperator:
    //         printf("operator: %s", node->name);
    //         break;
    //
    //     default:
    //         break;
    // }
    // printf("\n");
    //
    // if (node->is_leaf) {
    //     return;
    // }
    //
    // for(int i = 0;i < node->nodes->length; i++) {
    //     ExpresionNode *child_node = vector_get(node->nodes, i);
    //     print_tree(child_node, depth + 1);
    // }
}

char *get_node_name_for_number(double number) {
    // max length 31 items
    char *name = calloc(32, sizeof(char));
    sprintf(name, "%lf", number);

    return name;
}

void remove_nodes(Vector *node_nodes, Vector *to_remove_indexes) {
    for(int i = 0; i < to_remove_indexes->length; i++) {
        int *index_ptr = (int*)vector_get(to_remove_indexes, i);
        int index = *index_ptr;
        // subtract i to offset because length changes when we 
        // remove nodes
        ExpresionNode *removed_node = vector_remove(node_nodes, index - i);

        free(removed_node->name);
        free(removed_node);
        free(index_ptr);
    }
}

ExpresionNode *create_new_node(enum LexerTokenType type, char *name) {
    ExpresionNode *node = calloc(1, sizeof(ExpresionNode));

    int name_length = strlen(name);
    node->name = calloc(name_length + 1, sizeof(char));
    strcpy(node->name, name);

    node->type = type;
    node->value = 0;

    switch (type) {
        case TokenTypeNumber:
        case TokenTypeVariable:
            node->is_leaf = true;
            node->nodes = NULL;
            node->value = atof(node->name);
            break;
        default:
            node->nodes = vector_new();
            node->is_leaf = false;
    }

    return node;
}

ExpresionNode *create_new_number_node(double value) {
    char *number_value = calloc(100, sizeof(char));
    sprintf(number_value, "%lf", value);
    printf("number -> %lf\n", value);
    ExpresionNode *other_node = create_new_node(TokenTypeNumber, number_value);
    free(number_value);

    printf("other_node -> %p\n", other_node);
    other_node->value = value;

    return other_node;
}




void compact_operators(ExpresionNode *node) {
    if (node->is_leaf) {
        return;
    }

    switch (node->type) {
        case TokenTypeOperator:
            break;
        default:
            return;
    }

    for (int i = 0;i < node->nodes->length; i++) {
        compact_operators(vector_get(node->nodes, i));
    }

    switch (*node->name) {
        case '+':
            compact_add(node);
            break;
        case '-':
            compact_sub(node);
            break;
        case '*':
            compact_multiply(node);
            break;
        default:
            return;
    }
}

void flatten_tree(ExpresionNode *node) {
    if (node->is_leaf) {
        return;
    }

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);
        flatten_tree(child);
    }

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        bool nodes_have_the_same_name = strcmp(node->name, child->name) == 0;

        if (!nodes_have_the_same_name) {
            continue;
        }

        if (child->type != node->type) {
            continue;
        }

        if (child->is_leaf) {
            // this shouldnt ever trigger but whatever
            // since we check if these guys are of the same types
            // and at the top if node.is_leaf is true we return
            // only variables and constants SHOULD be leaf nodes
            continue;
        }


        for(int j = 0; j < node->nodes->length; j++) {
            if(j == i) {
                continue;
            }

            vector_pushback(
                    child->nodes,
                    vector_get(node->nodes, j)
            );
        }
        vector_free(node->nodes);
        node->nodes = child->nodes;
        free(child);
    }
}

ExpresionNode *convert_rpn_tokens_to_tree(Vector *tokens, Evaluator *evaluator) {
    Vector *node_stack = vector_new();

    for(int i = 0; i < tokens->length; i++) {
        LexerToken *token = vector_get(tokens, i);

        ExpresionNode *node = calloc(sizeof(ExpresionNode), 1);

        int name_length = 0;
        char old_end_char = *token->end_ptr;

        if(token->end_ptr == token->start_ptr) {
            name_length = 2;
            node->name[0] = *token->start_ptr;
            node->name[1] = '\0';
            printf("found name: %c\n", *token->end_ptr); 
        } else {
            *token->end_ptr = '\0';
            name_length = strlen(token->start_ptr) + 1;
        }

        node->name = calloc(name_length, sizeof(char));
        strcpy(node->name, token->start_ptr);

        *token->end_ptr = old_end_char;
        node->value = 0;
        node->type = token->type;

        switch (token->type) {
            case TokenTypeNumber:
                node->value = atof(node->name);
            case TokenTypeVariable:
                node->is_leaf = true;
                node->nodes = NULL;

                vector_pushback(node_stack, node);
                break;

            case TokenTypeFunction:
            case TokenTypeOperator:
            case TokenTypeUnaryOperator:
                int argument_count = 0;

                if (token->type == TokenTypeFunction) {
                    MathFunction *function = evaluator_get_function(
                        evaluator,
                        token
                    );
                    argument_count = function->argument_count;
                } else {
                    MathOperator *operation = evaluator_get_operation(
                        evaluator,
                        token,
                        token->type == TokenTypeUnaryOperator
                    );
                    argument_count = operation->argument_count;
                }

                node->is_leaf = false;
                node->nodes = vector_new();

                for(int j = argument_count; j > 0; j--) {
                    vector_pushback(
                        node->nodes,
                        vector_get(node_stack, node_stack->length - j)
                    );
                }
                for(int j = argument_count; j > 0; j--) {
                    vector_pop(node_stack);
                }
                vector_pushback(node_stack, node);
                break;
            default:
                    break;
        }
    }

    return vector_pop(node_stack);
}
