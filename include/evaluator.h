#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char operator;
    int precedence;
    int argument_count;

    bool is_unary;
    bool is_important;

    bool left_associative;
    bool (*callback)(Vector *stack);
} MathOperator;

typedef struct {
    char *name;
    int argument_count;
    bool (*callback)(Vector *stack);
} MathFunction;

typedef struct {
    char *name;
    double value;
} MathVariable;

typedef Vector VectorOfVariables;
typedef Vector VectorOfOperators;
typedef Vector VectorOfFunctions;

typedef struct {
    VectorOfVariables *variables;
    VectorOfOperators *operators;
    VectorOfFunctions *functions;
} Evaluator;

enum EvaluationErrors {
    ErrorInvalidEquation,
    ErrorNoFunction,
    ErrorNoOperator,
    ErrorNoUnaryOperator,
    ErrorNoVariable,
};

typedef struct {
    double result;
    bool success;
    int error_code;
} EvaluationResult;


EvaluationResult evaluate(Evaluator *evaluator, Vector *rpn_tokens);

Evaluator evaluator_new();

void evaluator_free(Evaluator *evaluator);
MathOperator *evaluator_get_operation(Evaluator *evaluator, const LexerToken *token, bool is_unary);
MathOperator *evaluator_get_operator(Evaluator *evaluator, const LexerToken *token);
MathOperator *evaluator_get_unary_operator(Evaluator *evaluator, const LexerToken *token);
MathVariable *evaluator_get_variable(Evaluator *evaluator, const LexerToken *token);
MathFunction *evaluator_get_function(Evaluator *evaluator, const LexerToken *token);

void evaluator_create_operator(
        Evaluator *evaluator,
        char operator,
        int precedence,
        int argument_count,
        bool left_associative,
        bool (*callback) (Vector*)
);
void evaluator_create_function(Evaluator *evaluator, const int argument_count, const char *operator, bool (*callback)(Vector *));
void evaluator_create_variable(Evaluator *evaluator, const char *name, double value);
void evaluator_create_unary_operator(
        Evaluator *evaluator,
        const char operator,
        const int precedence,
        const bool is_important,
        bool (*callback)(Vector *)
);

Vector *evaluator_tokenize(Evaluator *evaluator, char *input);


#endif
