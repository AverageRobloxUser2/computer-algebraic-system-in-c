#include "simplifier.h"
#include "hash_table.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evaluator.h"
#include <limits.h>

char *node_to_string(ExpresionNode *node) {
    char *result = calloc(100, sizeof(char));
    switch (node->type) {
        case TokenTypeUnaryOperator:
            sprintf(result, "unary_%s(", node->name);
            break;

        case TokenTypeOperator:
            sprintf(result, "op_%s(", node->name);
            break;

        case TokenTypeFunction:
            sprintf(result, "func_%s(", node->name);
            break;

        case TokenTypeVariable:
            sprintf(result, "var(\"%s\")", node->name);
            break;

        default:
            sprintf(result, "%s", node->name);
            break;
    }
    if (node->is_leaf) {
        return result;
    }

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);
        int old_length = strlen(result);

        char *child_string = node_to_string(child);
        int child_length = strlen(child_string);

        result = reallocarray(
            result,
            old_length + child_length + 2,
            sizeof(char)
        );

        strcpy(result + old_length, child_string);
        free(child_string);
        result[child_length + old_length] = ',';
        result[child_length + old_length + 1] = '\0';
    }

    int old_length = strlen(result);

    // result = reallocarray(result, old_length + 2, sizeof(char));
    result[old_length - 1] = ')';
    result[old_length] = '\0';

    return result;
}

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

void remove_nodes(Vector *nodes, Vector *to_remove) {
    // expects that to_remove values are casted into void ptrs
    // example
    // int x = 5;
    // vector_pushback(to_remove, (void*)x)
    for(int i = 0; i < to_remove->length; i++) {
        // because we cast the insert into a void* now we cast into int
        int index = (int)vector_get(to_remove, i);
        ExpresionNode *removed_node = vector_remove(nodes, index - i);

        free(removed_node->name);
        free(removed_node);
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

void compact_add(ExpresionNode *node) {
    printf("compacting\n");
    if (node->is_leaf) {
        return;
    }

    HashTable hash_table_counts = new_hash_table();
    HashTable hash_table_nodes = new_hash_table();


    bool has_sum = false;
    double sum = 0;

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (child->type == TokenTypeNumber) {
            sum += child->value;
            has_sum = true;
            printf("found sum\n");
            continue;
        }

        char *child_expression = node_to_string(child);
        int *previous_value = get_hash_table(&hash_table_counts, child_expression);

        if (previous_value == NULL) {
            previous_value = malloc(sizeof(int));
            *previous_value = 0;
            set_hash_table(&hash_table_counts, child_expression, previous_value);
            ExpresionNode *previous_node = get_hash_table(&hash_table_nodes, child_expression);
            free(previous_node);
        }

        // cuz hash table stores pointers increasing this increases
        // the value inside of the hash table
        (*previous_value)++;
        set_hash_table(&hash_table_nodes, child_expression, child);
    }

    // remove all children
    while (node->nodes->length > 0) {
        vector_pop(node->nodes);
    }

    for(int i = 0; i < hash_table_counts.current_filled_count; i++) {
        char *key = hash_table_counts.keys[i];
        int *count = get_hash_table(&hash_table_counts, key);

        ExpresionNode *original_node = get_hash_table(&hash_table_nodes, key);
        if(*count == 1) {
            vector_pushback(node->nodes, original_node);
            continue;
        }

        ExpresionNode *multiply_node = create_new_node(TokenTypeOperator, "*");
        ExpresionNode *other_node = create_new_number_node((double)*count); 

        vector_pushback(multiply_node->nodes, other_node);
        vector_pushback(multiply_node->nodes, original_node);
        printf("yay yay ya\n");
        print_tree(multiply_node, 0);
        vector_pushback(node->nodes, multiply_node);
        print_tree(node, 0);
    }

    if (has_sum) {
        ExpresionNode *other_node = create_new_number_node(sum);
        vector_pushback(node->nodes, other_node);
    }
}

void compact_sub(ExpresionNode *node) {
    Vector *to_remove = vector_new();
    double result = 0;
    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (child->type != TokenTypeNumber) {
            continue;
        }

        if(i == 0) {
            result += child->value;
        } else {
            result -= child->value;
        }

        // very weird hack we cast to void* so we can then uncast to (int) 
        // because the vector_pushback signature is Vector* and void*
        vector_pushback(to_remove, (void*)i);
    }
    remove_nodes(node->nodes, to_remove);
    vector_free(to_remove);
    ExpresionNode *new_node;

    if (node->nodes->length == 0) {
        vector_free(node->nodes);
        node->nodes = NULL;

        new_node = node;
    } else {
        new_node = calloc(1, sizeof(ExpresionNode));
        vector_pushback(node->nodes, new_node);
    }

    new_node->is_leaf = true;
    new_node->name = get_node_name_for_number(result);
    new_node->type = TokenTypeNumber;
    new_node->value = result;
}

void compact_multiply(ExpresionNode *node) {
    Vector *to_remove = vector_new();
    double result = 1;
    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (child->type != TokenTypeNumber) {
            continue;
        }

        result *= child->value;

        // very weird hack we cast to void* so we can then uncast to (int) 
        // because the vector_pushback signature is Vector* and void*
        vector_pushback(to_remove, (void*)i);
    }

    remove_nodes(node->nodes, to_remove);
    vector_free(to_remove);
    ExpresionNode *new_node;

    if (node->nodes->length == 0) {
        vector_free(node->nodes);
        node->nodes = NULL;

        new_node = node;
    } else {
        new_node = calloc(1, sizeof(ExpresionNode));
        vector_pushback(node->nodes, new_node);
    }

    new_node->is_leaf = true;
    new_node->name = get_node_name_for_number(result);
    new_node->type = TokenTypeNumber;
    new_node->value = result;
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

ExpresionNode *convert_to_tree(Vector *tokens, Evaluator *evaluator) {
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
