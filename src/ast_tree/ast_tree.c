#include "ast_tree.h"
#include "math_equation.h"
#include "stdlib.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

AstNode *create_new_node(
    MathEquationTokenType type,
    char *name) {

    AstNode *node = malloc(sizeof(AstNode));

    node->type = type;
    char *new_name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(new_name, name);

    node->name = new_name;

    if (type == MathNumberToken) {
        node->value = atof(name);
    } else {
        node->value = 0;
    }

    node->children_ptrs = NULL;
    node->child_count = 0;

    return node;
}

void append_child_node(AstNode *parent, AstNode *child) {
    parent->child_count ++;
    parent->children_ptrs = reallocarray(
        parent->children_ptrs,
        parent->child_count,
        sizeof(AstNode*)
    );

    parent->children_ptrs[parent->child_count-1] = child;
}

void free_ast(AstNode *node) {
    for (size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        free_ast(child);
    }

    free(node->name);
    free(node->children_ptrs);
    free(node);
}

char *ast_node_to_string(AstNode *node) {
    char *result = calloc(100, sizeof(char));
    switch (node->type) {
        case MathFunctionToken:
        case MathUnaryOperatorToken:
        case MathOperatorToken:
            sprintf(result, "%s(", node->name);
            break;
        case MathVariableToken:
        case MathNumberToken:
            sprintf(result, "%s", node->name);
            return result;
        default:
            printf("Invalid node type encountered\n");
            exit(1);
    }
    size_t result_length = strlen(result);
    result = reallocarray(result, result_length + 1, sizeof(char));

    for(size_t i = node->child_count; i > 0; i--) {
        AstNode *child = node->children_ptrs[i-1];

        char *child_to_string = ast_node_to_string(child);
        // old length + child_length + , + end
        size_t new_length = result_length + strlen(child_to_string) + 1 + 1;

        result = reallocarray(result, new_length, sizeof(char));
        strcpy(result + result_length, child_to_string);
        result[new_length-2] = ',';

        result_length = strlen(result);
        free(child_to_string);
    }

    // the last character is always ',', since we append ',' to the result
    // in the for loop
    result[result_length-1] = ')';

    return result;
}
