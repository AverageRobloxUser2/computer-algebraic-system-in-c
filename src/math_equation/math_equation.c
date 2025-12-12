#include "math_equation.h"
#include <stdlib.h>
#include <string.h>

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
