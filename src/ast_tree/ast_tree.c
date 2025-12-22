#include "ast_tree.h"
#include "lexer.h"
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
    node->name = NULL;
    node->children_ptrs = NULL;
    free(node);
}

char *ast_node_to_string(AstNode *node) {
    char *result = calloc(100, sizeof(char));
    switch (node->type) {
        case MathFunctionToken:
        case MathOperatorToken:
            sprintf(result, "%s(", node->name);
            break;
        case MathUnaryOperatorToken:
            sprintf(result, "u_%s(", node->name);
            break;
        case MathVariableToken:
        case MathNumberToken:
            sprintf(result, "%s", node->name);
            return result;
        default:
            printf("Invalid node type (%d) encountered\n", node->type);
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

AstNode *string_to_ast_node(char *input) {
    char *writable_input = calloc(strlen(input) + 1, sizeof(char));
    strcpy(writable_input, input);

    LexerResult lexer_result = lex(writable_input);
    free(writable_input);

    InfixEquation infix = convert_lexed_to_infix(lexer_result);
    free_lexed(lexer_result);

    PostfixEquation postfix = convert_infix_to_postfix(infix);
    free_equation(infix);

    AstNode *node = postfix_to_ast(postfix);
    free_equation(postfix);

    ast_node_concat_operators(node);

    return node;
}


AstNode *deep_clone_node(AstNode *node) {
    AstNode *result = create_new_node(node->type, node->name);

    for(size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        append_child_node(result, deep_clone_node(child));
    }
    
    return result;
}

void append_combination(
        char*** combinations_ptr,
        size_t *combination_count_ptr,
        char *combination
) {
    char *new_combination = calloc(strlen(combination) + 1, sizeof(char));
    strcpy(new_combination, combination);
    (*combination_count_ptr)++;
    *combinations_ptr = reallocarray(
        *combinations_ptr,
        *combination_count_ptr,
        sizeof(char*)
    );
    (*combinations_ptr)[(*combination_count_ptr) - 1] = new_combination;
}

char *ast_node_to_equation(AstNode *node) {
    char ** combinations = NULL;
    size_t combination_count = 0;

    switch (node->type) {
        case MathVariableToken:
        case MathNumberToken:
            append_combination(
                &combinations,
                &combination_count,
                node->name
            );
            break;
        case MathOperatorToken:
        case MathFunctionToken:
        case MathUnaryOperatorToken:
            append_combination(
                &combinations,
                &combination_count,
                "("
            );
            char *first_equation = ast_node_to_equation(node->children_ptrs[0]);
            append_combination(
                &combinations,
                &combination_count,
                first_equation
            );
            free(first_equation);
            for(size_t i = 1; i < node->child_count; i++) {
                AstNode *child = node->children_ptrs[i];
                append_combination(
                    &combinations,
                    &combination_count,
                    node->name
                );
                char *equation = ast_node_to_equation(child);
                append_combination(
                    &combinations,
                    &combination_count,
                    equation
                );
                free(equation);
            }
            append_combination(
                &combinations,
                &combination_count,
                ")"
            );
    }

    char *result = NULL;
    size_t result_length = 0;


    for(size_t i = 0; i < combination_count; i++) {
        char *combination = combinations[i];
        size_t old_length = result_length;
        result_length = result_length + strlen(combination);
        result = reallocarray(result, result_length + 1, sizeof(char));
        strcpy(result + old_length, combination);
        free(combination);
    }

    free(combinations);

    return result;
}
