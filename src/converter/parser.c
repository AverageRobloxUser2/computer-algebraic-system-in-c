#include "parser.h"
#include "evaluator.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Vector *parse(Vector *tokens, Evaluator *evaluator) {
    Vector *result = vector_new();
    Vector *operator_stack = vector_new();

    for(int i = 0; i < tokens->length; i++) {
        LexerToken *token = vector_get(tokens, i);

        if (token->type == TokenTypeNumber || token->type == TokenTypeVariable) {
            vector_pushback(result, token);
            continue;
        } else if (token->type == TokenTypeOperator || token->type == TokenTypeUnaryOperator) {
            // if token is operator
            //  1. always append current operator to operator stack
            //  2. if current operators precedence is lower then previous
            //
            //      then we append previous operator to result


            LexerToken *last_token = vector_get(operator_stack, operator_stack->length - 1);
            if (last_token == NULL) {
                goto submit; 
            }

            if (last_token->type == TokenTypeFunction) {
                goto yay;
            }

            if (last_token->type != TokenTypeOperator && last_token->type != TokenTypeUnaryOperator) {
                goto submit;
            }

            MathOperator *last_operation = evaluator_get_operation 
                (evaluator, last_token, last_token->type == TokenTypeUnaryOperator);
            MathOperator *operation = evaluator_get_operation(evaluator, token, token->type == TokenTypeUnaryOperator);

            if (last_operation == NULL) {
                goto submit;
            }
            if (operation == NULL) {
                goto submit;
            }

            if (last_operation->precedence < operation->precedence) {
                goto submit; 
            }

            if (!operation->left_associative) {
                if (last_operation->precedence == operation->precedence) {
                    goto submit;
                }
            }

            // you get here if
            // #1 last operation precedence is higher or equal to current precedence
            // #2 if precedence equal then the current operation is left_associative

            yay:
            vector_pushback(result, last_token);
            vector_pop(operator_stack);

            submit:
            vector_pushback(operator_stack, token);

            continue;
        } else if (token->type == TokenTypeFunction) {
            vector_pushback(operator_stack, token);
        } else if (token->type == TokenTypeParenthesis) {
            if (*(token->start_ptr) == '(') {

                vector_pushback(operator_stack, token);
                continue;
            }
            while (true) {
                LexerToken *last_token = vector_pop(operator_stack);

                if (last_token == NULL) {
                    break;
                }

                if (*(last_token->start_ptr) == '(') {
                    break;
                }

                vector_pushback(result, last_token);
            }
        }
    }


    while (operator_stack->length > 0) {
        LexerToken *last_token = vector_pop(operator_stack);

        if (last_token == NULL) {
            break;
        }

        vector_pushback(result, last_token);
    }

    vector_free(operator_stack);

    return result;
};
