#ifndef MATH_EQUATION_H
#define MATH_EQUATION_H


#include "lexer.h"
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


MathEquationTokenType convert_type(enum LexerTokenType type);

MathEquationToken new_token(MathEquationTokenType type, char *value);
void add_token(MathEquation *equation, MathEquationToken token);


bool is_value_token(MathEquationToken token);
void free_equation(MathEquation equation);

#endif
