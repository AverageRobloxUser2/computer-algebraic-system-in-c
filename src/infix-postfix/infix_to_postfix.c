#include "math_equation.h"
#include "postfix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

MathEquationToken pop_from_operator_stack(
    MathEquationToken **operator_stack_ptr,
    size_t *operator_stack_count_ptr
) {
    MathEquationToken last_value = (*operator_stack_ptr)[*operator_stack_count_ptr - 1];
    (*operator_stack_count_ptr)--;
    if (*operator_stack_count_ptr > 0) {

        *operator_stack_ptr = reallocarray(
            *operator_stack_ptr,
            (*operator_stack_count_ptr),
            sizeof(MathEquationToken)
        );

    } else {
        free(*operator_stack_ptr);
        *operator_stack_ptr =  NULL;
    }

    return last_value;
}

void append_to_operator_stack(
    MathEquationToken **operator_stack_ptr,
    size_t *operator_stack_count_ptr,
    MathEquationToken new_value
) {
    *operator_stack_ptr = reallocarray(
        *operator_stack_ptr,
        *operator_stack_count_ptr+1,
        sizeof(MathEquationToken)
    );

    (*operator_stack_ptr)[*operator_stack_count_ptr] = new_value;
    (*operator_stack_count_ptr)++;
}

void append_to_result(PostfixEquation *result, MathEquationToken token) {
    add_token(
        result,
        new_token(
            token.type,
            token.value
        )
    );
}


PostfixEquation convert_infix_to_postfix(InfixEquation equation) {
    PostfixEquation result;
    result.tokens = NULL;
    result.token_count = 0;
    result.success = false;

    MathEquationToken *operator_stack = NULL;
    size_t operator_stack_count = 0;


    for(size_t i = 0; i < equation.token_count; i++) {
        // printf("%zu.\n", i);
        // printf("\tOP STACK:");
        // for(size_t j = 0; j < operator_stack_count; j++) {
        //     printf("%s,", operator_stack[j].value);
        // }
        // printf("\n");
        // printf("\tRESULT:");
        // for(size_t j = 0; j < result.token_count; j++) {
        //     printf("%s,", result.tokens[j].value);
        // }
        // printf("\n");
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
                append_to_operator_stack(
                    &operator_stack,
                    &operator_stack_count,
                    token
                );
                continue;
            }

            MathEquationToken previous_operator = operator_stack[operator_stack_count-1];

            if (previous_operator.type == MathFunctionToken) {
                append_to_result(
                    &result, 
                    pop_from_operator_stack(&operator_stack, &operator_stack_count)
                );
                continue;
            }

            if (previous_operator.type != MathOperatorToken
                    && previous_operator.type != MathUnaryOperatorToken) {
                append_to_operator_stack(
                    &operator_stack,
                    &operator_stack_count,
                    token
                );

                continue;
            }

            size_t current_precedence = get_precedence_for_operator(*token.value);
            size_t previous_precedence = get_precedence_for_operator(
                *previous_operator.value
            );

            if (previous_precedence < current_precedence) {
                append_to_operator_stack(
                    &operator_stack,
                    &operator_stack_count,
                    token
                );
                continue;
            }

            bool right_associative = !is_left_associative(*token.value);
            if (token.type == MathUnaryOperatorToken) {
                right_associative = true;
            }
            if (right_associative &&
                current_precedence == previous_precedence
            ){
                append_to_operator_stack(
                    &operator_stack,
                    &operator_stack_count,
                    token
                );
                continue;
            }

            while(operator_stack_count > 0) {
                MathEquationToken last_token = pop_from_operator_stack(
                    &operator_stack, 
                    &operator_stack_count
                );
                size_t last_precedence = get_precedence_for_operator(*last_token.value);

                // printf("last %zu, new %zu\n", last_precedence, current_precedence);
                if (last_precedence >= current_precedence) {
                    append_to_result(&result, last_token);
                    continue;
                }

                append_to_operator_stack(&operator_stack, &operator_stack_count, last_token);
                break;
            }

            append_to_operator_stack(&operator_stack, &operator_stack_count, token);

            // pop_from_operator_stack_and_append_to_result:
            //     operator_stack = reallocarray(
            //         operator_stack,
            //         operator_stack_count-1,
            //         sizeof(MathEquationToken)
            //     );
            //     operator_stack_count--;
            //
            //     add_token(
            //         &result,
            //         new_token(
            //             previous_operator.type,
            //             previous_operator.value
            //         )
            //     );
            //     goto append_to_operator_stack;
            //
            // append_to_operator_stack:
            //     operator_stack = reallocarray(
            //         operator_stack,
            //         operator_stack_count+1,
            //         sizeof(MathEquationToken)
            //     );
            //
            //     operator_stack[operator_stack_count] = token;
            //     operator_stack_count++;
            //
            continue;
        } else if (token.type == MathFunctionToken) {
            append_to_operator_stack(&operator_stack, &operator_stack_count, token);
        } else if (token.type == MathParenthasisOpenToken) {
            append_to_operator_stack(&operator_stack, &operator_stack_count, token);
        } else if (token.type == MathParenthasisClosedToken) {
            while (true) {
                if (operator_stack_count == 0) {
                    break;
                }

                MathEquationToken last_token = pop_from_operator_stack(
                    &operator_stack,
                    &operator_stack_count
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
        MathEquationToken last_token = pop_from_operator_stack(
            &operator_stack,
            &operator_stack_count
        );
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
