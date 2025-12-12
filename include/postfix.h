#ifndef POSTFIX_H
#define POSTFIX_H

#include <stdbool.h>
#include <stddef.h>
typedef enum {
    MathUnaryOperatorToken,
    MathOperatorToken,
    MathFunctionToken,

    MathVariableToken,
    MathNumberToken,

    MathParenthasisOpenToken,
    MathParenthasisClosedToken,
    MathWrongToken
} MathEquationTokenType;

typedef struct {
    MathEquationTokenType type;
    char *value;
} MathEquationToken;

typedef struct {
    MathEquationToken *tokens;
    size_t token_count;
    bool success;
} MathEquation;

typedef MathEquation PostfixEquation;

#endif
