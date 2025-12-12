#include "infix.h"
#include "lexer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void do_implicit_multiplication_checks(
    InfixEquation *result_infix,
    InfixEquation previous_infix,
    size_t i
) {
    MathEquationToken token = previous_infix.tokens[i];
    if(i == 0) {
        // if we at the first token just add it breh
        // this case trigged for equations such as
        // "sin(x)", "(2+1)*2"
        // but not triggered for
        // "2+2"
        return;
    }

    MathEquationToken previous_token = previous_infix.tokens[i-1];

    if (previous_token.type == MathParenthasisClosedToken &&
            token.type != MathOperatorToken) {
        // triggers for cases like this.
        // ...)2 or ...)sin(2) or ...)(... but not for ...)+x
        add_token(
            result_infix,
            new_token(
                MathOperatorToken,
                "*"
            )
        );

        return;
    }
    if (previous_token.type != MathOperatorToken && 
            token.type == MathParenthasisOpenToken) {

        add_token(
            result_infix,
            new_token(
                MathOperatorToken,
                "*"
            )
        );

        return;
    }

    if (previous_token.type == MathOperatorToken && 
        token.type != MathOperatorToken &&
        *previous_token.value == '!'
        ) {
        // triggers for cases like 5!2
        // which should result in 5! * 2
        add_token(
            result_infix,
            new_token(
                MathOperatorToken,
                "*"
            )
        );

        return;
    }

    if (is_value_token(previous_token) && is_value_token(token)) {
        add_token(
            result_infix,
            new_token(
                MathOperatorToken,
                "*"
            )
        );
    }
}

InfixEquation convert_lexed_to_infix_only_division(LexerResult lexed) {
    InfixEquation infix_result;

    infix_result.success = false;
    infix_result.token_count = 0;
    infix_result.tokens = NULL;

    bool waiting_to_close = false;

    for(size_t i = 0; i < lexed.token_count; i++) {
        LexerToken token = lexed.tokens[i];
        if (token.type == TokenTypeOperator) {
            if (!waiting_to_close && *token.value == '/') {
                add_token(
                    &infix_result,
                    new_token(
                        convert_type(token.type),
                        token.value
                    )
                );
                add_token(
                    &infix_result,
                    new_token(
                        MathParenthasisOpenToken,
                        "("
                    )
                );
                waiting_to_close = true;
                continue;
            }
            if (waiting_to_close ) {
                waiting_to_close = false;
                add_token(
                    &infix_result,
                    new_token(
                        MathParenthasisClosedToken,
                        ")"
                    )
                );
            }
        }
        
        add_token(
            &infix_result,
            new_token(
                convert_type(token.type),
                token.value
            )
        );
    }

    if (waiting_to_close) {
        add_token(
            &infix_result,
            new_token(
                MathParenthasisClosedToken,
                ")"
            )
        );
    }

    return infix_result;
}

InfixEquation convert_lexed_to_infix_multiplication(InfixEquation previous_infix) {
    InfixEquation infix_result;

    infix_result.success = false;
    infix_result.token_count = 0;
    infix_result.tokens = NULL;

    for(size_t i = 0; i < previous_infix.token_count; i++) {
        MathEquationToken token = previous_infix.tokens[i];
        bool is_value = is_value_token(token);

        do_implicit_multiplication_checks(
            &infix_result,
            previous_infix,
            i
        );

        add_token(
            &infix_result,
            new_token(
                token.type,
                token.value
            )
        );
    }

    return infix_result;
}

InfixEquation convert_lexed_to_infix_unary(InfixEquation previous_infix) {
    InfixEquation infix_result;

    infix_result.success = false;
    infix_result.token_count = 0;
    infix_result.tokens = NULL;

    bool waiting_for_value = true;

    for(size_t i = 0;i < previous_infix.token_count; i++) {
        MathEquationToken token = previous_infix.tokens[i];

        if (token.type == MathParenthasisOpenToken && 
        token.type == MathParenthasisClosedToken ) {
            add_token(
                &infix_result,
                new_token(
                    token.type,
                    token.value
                )
            );
            continue;
        }

        if (is_value_token(token)) {
            waiting_for_value = false;
            add_token(
                &infix_result,
                new_token(
                    token.type,
                    token.value
                )
            );
            continue;
        }     

        if (waiting_for_value) {
            add_token(
                &infix_result,
                new_token(
                    MathUnaryOperatorToken,
                    token.value
                )
            );
        } else {
            waiting_for_value = true;
            add_token(
                &infix_result,
                new_token(
                    MathUnaryOperatorToken,
                    token.value
                )
            );
        }
    }

    return infix_result;
}

InfixEquation convert_lexed_to_infix(LexerResult lexed) {
    // adds implicit multiplication, convert to unary operators, etc
    // this will then be used to for converting to postfix 
    bool expecting_value = true;
    InfixEquation only_division = convert_lexed_to_infix_only_division(lexed);
    InfixEquation without_unary = convert_lexed_to_infix_multiplication(only_division);
    InfixEquation result = convert_lexed_to_infix_unary(without_unary);

    return result;
}
