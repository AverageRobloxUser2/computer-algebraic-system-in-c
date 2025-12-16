#include "math_equation.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

bool is_left_associative(char operator) {
    if (operator == '^') {
        return false;
    }
    if (operator == '!') {
        return false;
    }

    return true;
}

size_t get_precedence_for_operator(char operator) {
    switch (operator) {
        case '!':
            return 4;
        case '^':
            return 4;

        case '*':
        case '/':
            return 3;

        case '+':
        case '-':
            return 2;
        default:
            return 1;
    }
}

MathEquationTokenType convert_type(enum LexerTokenType type) {
    switch (type) {
        case TokenTypeOperator:
            return MathOperatorToken;

        case TokenTypeNumber:
            return MathNumberToken;

        case TokenTypeVariable:
            return MathVariableToken;

        case TokenTypeFunction:
            return MathFunctionToken;

        case TokenTypeParenthesisOpen:
            return MathParenthasisOpenToken;

        case TokenTypeParenthesisClosed:
            return MathParenthasisClosedToken;

        default:
            return MathWrongToken;
    }
}

MathEquationToken new_token(MathEquationTokenType type, char *value) {
    MathEquationToken token;

    token.type = type;
    token.value = calloc(strlen(value) + 1, sizeof(char));

    strcpy(token.value, value);

    return token;
}

void add_token(MathEquation *equation, MathEquationToken token) {
    equation->token_count++;
    equation->tokens = reallocarray(
        equation->tokens,
        equation->token_count,
        sizeof(MathEquationToken)
    );
    equation->tokens[equation->token_count-1] = token;
}

bool is_value_token(MathEquationToken token) {
    return token.type == MathNumberToken || token.type == MathVariableToken;
}

void free_equation(MathEquation equation) {
    for(size_t i = 0;i < equation.token_count; i++) {
        MathEquationToken token = equation.tokens[i];
        free(token.value);
    }
    free(equation.tokens);
}

size_t get_argument_count_for_token(MathEquationToken token) {
    switch (token.type) {
        case MathOperatorToken:
            return 2;
        case MathUnaryOperatorToken:
        case MathFunctionToken:
            return 1;
        default:
            return -1;
    }
}
