#include "ast_tree.h"
#include "math_equation.h"
#include "postfix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


PostfixEquation convert_infix_to_postfix(InfixEquation equation) {
    PostfixEquation result;
    result.tokens = NULL;
    result.token_count = 0;
    result.success = false;

    MathEquationToken *operator_stack = NULL;
    size_t operator_stack_count = 0;


    for(size_t i = 0; i < equation.token_count; i++) {
        MathEquationToken token = equation.tokens[i];

        if (token.type == MathNumberToken || token.type == MathVariableToken) {
            add_token(
                &result,
                new_token(
                    token.type,
                    token.value
                )
            );
            continue;
        } else if (token.type == MathOperatorToken || token.type == MathUnaryOperatorToken) {
            if (operator_stack_count < 1) {
                goto append_to_operator_stack;
            }

            MathEquationToken previous_token = operator_stack[operator_stack_count-1];

            if (previous_token.type == MathFunctionToken) {
                goto pop_from_operator_stack_and_append_to_result;
            }

            if (previous_token.type != MathOperatorToken) {

                goto append_to_operator_stack;
            }

            size_t current_precedence = get_precedence_for_operator(*token.value);
            size_t previous_precedence = get_precedence_for_operator(
                *previous_token.value
            );

            if (previous_precedence < current_precedence) {
                goto append_to_operator_stack;
            }

            if (!is_left_associative(*token.value) &&
                current_precedence == previous_precedence
            ){
                goto append_to_operator_stack;
            }
            pop_from_operator_stack_and_append_to_result:
                operator_stack = reallocarray(
                    operator_stack,
                    operator_stack_count-1,
                    sizeof(MathEquationToken)
                );
                operator_stack_count--;

                add_token(
                    &result,
                    new_token(
                        previous_token.type,
                        previous_token.value
                    )
                );
                goto append_to_operator_stack;

            append_to_operator_stack:
                operator_stack = reallocarray(
                    operator_stack,
                    operator_stack_count+1,
                    sizeof(MathEquationToken)
                );

                operator_stack[operator_stack_count] = token;
                operator_stack_count++;

            continue;
        } else if (token.type == MathFunctionToken) {
            operator_stack = reallocarray(
                operator_stack,
                operator_stack_count+1,
                sizeof(MathEquationToken)
            );

            operator_stack[operator_stack_count] = token;
            operator_stack_count++;
        } else if (token.type == MathParenthasisOpenToken) {
            operator_stack = reallocarray(
                operator_stack,
                operator_stack_count+1,
                sizeof(MathEquationToken)
            );

            operator_stack[operator_stack_count] = token;
            operator_stack_count++;
        } else if (token.type == MathParenthasisClosedToken) {
            while (true) {
                if (operator_stack_count == 0) {
                    break;
                }

                operator_stack_count--;
                MathEquationToken last_token = operator_stack[operator_stack_count];
                operator_stack = reallocarray(
                    operator_stack,
                    operator_stack_count,
                    sizeof(MathEquationToken)
                );


                if (last_token.type == MathParenthasisOpenToken) {
                    break;
                }

                add_token(
                    &result,
                    new_token(
                        last_token.type, last_token.value
                    )
                );

            }
        }
    }


    while (operator_stack_count > 0) {
        MathEquationToken last_token = operator_stack[operator_stack_count-1];
        operator_stack = reallocarray(
            operator_stack,
            operator_stack_count-1,
            sizeof(MathEquationToken)
        );
        operator_stack_count--;

        add_token(
            &result,
            new_token(
                last_token.type,
                last_token.value
            )
        );
    }

    result.success = true;
    return result;
}
