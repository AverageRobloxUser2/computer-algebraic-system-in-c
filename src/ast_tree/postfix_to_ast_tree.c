#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>



AstNode *pop_node_from_stack(AstNode ***node_stack_ptr, size_t *node_count_ptr) {

    (*node_count_ptr)--;
    AstNode *last_node = (*node_stack_ptr)[*node_count_ptr];
    *node_stack_ptr = reallocarray(*node_stack_ptr, *node_count_ptr, sizeof(AstNode*));

    return last_node;
}

void add_node_to_stack(AstNode ***node_stack_ptr, size_t *node_count_ptr, AstNode *node_ptr) {

    *node_stack_ptr = reallocarray(
        *node_stack_ptr,
        *node_count_ptr + 1,
        sizeof(AstNode*)
    );

    (*node_stack_ptr)[*node_count_ptr] = node_ptr;
    (*node_count_ptr)++;
}

void handle_functional_token(
        MathEquationToken token,
        AstNode ***node_stack_ptr,
        size_t *node_count_ptr
) {

    switch (token.type) {
        case MathFunctionToken:
        case MathUnaryOperatorToken:
        case MathOperatorToken:
            break;
        default:
            printf("Invalid functional node type\n");
            return;
    }

    AstNode *node = create_new_node(
        token.type,
        token.value
    );

    size_t argument_count = get_argument_count_for_token(token);

    if (argument_count > *node_count_ptr) {
        printf(
            "Not enough variables in stack for this token: \"%s\" -> %d\n", 
            token.value,
            token.type
        );
        printf("Expected argument count (%ld) found (%ld)\n", argument_count, *node_count_ptr);
        exit(1);
    }

    for(size_t i = 0;i < argument_count; i++) {
        append_child_node(
            node,
            pop_node_from_stack(
                node_stack_ptr,
                node_count_ptr
            )
        );
    }

    add_node_to_stack(node_stack_ptr, node_count_ptr, node);
}

AstNode *postfix_to_ast(PostfixEquation postfix) {
    AstNode **node_stack = NULL;
    size_t node_count = 0;

    for (size_t i = 0; i < postfix.token_count; i++) {
        MathEquationToken token = postfix.tokens[i];

        if (is_value_token(token)) {
            add_node_to_stack(
                &node_stack,
                &node_count,
                create_new_node(
                    token.type,
                    token.value
                )
            );
        } else {
            handle_functional_token(
                token,
                &node_stack,
                &node_count
            );
        }
    }

    if (node_count != 1) {
        printf(
            "Invalid amount of nodes found expected 1 got %ld\n",
            node_count
        );
        return NULL;
    }

    AstNode *result = pop_node_from_stack(&node_stack, &node_count);
    free(node_stack);

    return result;
}


