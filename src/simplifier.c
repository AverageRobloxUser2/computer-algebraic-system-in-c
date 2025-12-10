#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evaluator.h"
#include <limits.h>

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

void compact_operators(ExpresionNode *node) {
    // will compact leafs for operator 
    if (node->is_leaf) {
        return;
    }

    switch (node->token->type) {
        case TokenTypeOperator:
            break;
        default:
            return;
    }
    for (int i = 0;i < node->nodes->length; i++) {
        compact_operators(vector_get(node->nodes, i));
    }

    int new_value = INT_MAX;
    Vector *to_remove = vector_new();
    for (int i = 1;i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (!child->is_leaf) {
            continue;
        }

        if (child->token->type == TokenTypeVariable) {
            continue;
        }

        double value = strtod(child->token->start_ptr, &child->token->end_ptr);

        int value_rounded = (int) value;
        if (value - value_rounded != 0) {
            printf("skipping %lf since its a float %lf\n", value, value - value_rounded);
            continue;
        }

        int *to_remove_index = malloc(sizeof(int));
        *to_remove_index = i;
        vector_pushback(to_remove, to_remove_index);

        if (new_value == INT_MAX) {
            new_value = value_rounded;
            continue;
        }


        switch (*node->token->start_ptr) {
            case '+':
                new_value += value_rounded;
                break;
            case '-':
                new_value -= value_rounded;
                break;
            case '*':
            case '/':
                new_value *= value_rounded;
                break;
            default:
                // if node is not of suported type dont simplify
                return;
        }
    }

    if (to_remove->length == 1){
        return;
    }

    for(int i = 0; i < to_remove->length; i++) {
        int *index = vector_get(to_remove, i);
        ExpresionNode *child = vector_remove(node->nodes, *index - i);

        free(child);

        printf("removed %d\n", *index);
        free(index);
    }
    vector_free(to_remove);
    // omg this is so hacky
    char *content = calloc(sizeof(char), 32);
    ExpresionNode *new_node = malloc(sizeof(ExpresionNode));
    sprintf(content, "%d", new_value);
    new_node->is_leaf = true;
    new_node->token = malloc(sizeof(LexerToken));

    new_node->token->start_ptr = content;
    new_node->token->end_ptr = content + strlen(content);
    new_node->token->type = TokenTypeNumber;

    printf("New node: %p\n", new_node);


    vector_pushback(node->nodes, new_node);
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

                vector_pushback(node_stack, node);
                break;

            case TokenTypeFunction:
            case TokenTypeOperator:
            case TokenTypeUnaryOperator:
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
