#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evaluator.h"

void print_tree(ExpresionNode *node, int depth) {
    // starting depth is 1
    char *indent = calloc(sizeof(char), depth);

    for(int i = 0; i < depth - 1; i++) {
        indent[i] = '\t';
    }

    char old_end_value = *node->token->end_ptr;
    *node->token->end_ptr = '\0';

    printf("%s", indent);
    switch (node->token->type) {
        case TokenTypeVariable:
            printf("variable: %s", node->token->start_ptr);
            break;
        case TokenTypeNumber:
            printf("number: %s", node->token->start_ptr);
            break;
        case TokenTypeFunction:
            printf("function: %s", node->token->start_ptr);
            break;
        case TokenTypeOperator:
        case TokenTypeUnaryOperator:
            printf("operator: %s", node->token->start_ptr);
            break;

        default:
            break;
    }
    *node->token->end_ptr = old_end_value;
    printf("\n");

    free(indent);

    if (node->is_leaf) {
        return;
    }

    for(int i = 0;i < node->nodes->length; i++) {
        ExpresionNode *child_node = vector_get(node->nodes, i);

        print_tree(child_node, depth + 1);
    }
}


void flatten_tree(ExpresionNode *node) {
    if (node->is_leaf) {
        return;
    }

    char old_end_value = *node->token->end_ptr;

    *node->token->end_ptr = '\0';
    char *node_name = calloc(sizeof(char), strlen(node->token->start_ptr) + 1);
    strcpy(node_name, node->token->start_ptr);
    *node->token->end_ptr = old_end_value;

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);
        flatten_tree(child);
    }

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        old_end_value = *child->token->end_ptr;
        *child->token->end_ptr = '\0';
        char *child_name = calloc(sizeof(char), strlen(child->token->start_ptr) + 1);
        strcpy(child_name, child->token->start_ptr);
        *child->token->end_ptr = old_end_value;

        printf("names %s -> %s\n", node_name, child_name);
        bool nodes_have_the_same_name = strcmp(node_name, child_name) == 0;
        free(child_name);

        if (!nodes_have_the_same_name) {
            continue;
        }

        if (child->token->type != node->token->type) {
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

    free(node_name);
}

ExpresionNode *convert_to_tree(Vector *tokens, Evaluator *evaluator) {
    Vector *node_stack = vector_new();

    for(int i = 0; i < tokens->length; i++) {
        LexerToken *token = vector_get(tokens, i);

        ExpresionNode *node = calloc(sizeof(ExpresionNode), 1);

        switch (token->type) {
            case TokenTypeNumber:
            case TokenTypeVariable:
                node->is_leaf = true;
                node->nodes = NULL;
                node->token = token;

                printf("yay\n");
                vector_pushback(node_stack, node);
                break;

            case TokenTypeFunction:
            case TokenTypeOperator:
            case TokenTypeUnaryOperator:
                printf("nay\n");
                int argument_count;

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

                node->token = token;
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
