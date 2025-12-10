#include "evaluator.h"
#include "vector.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math_functions.h"
#include "math_operators.h"



EvaluationResult evaluate(Evaluator *evaluator, Vector *rpn_tokens) {
    Vector *stack = vector_new();

    EvaluationResult result;
    result.success = false;

    for(int i = 0;i < rpn_tokens->length; i++) {
        LexerToken *token = vector_get(rpn_tokens, i);

        switch (token->type) {
            case TokenTypeNumber:
                double *value = malloc(sizeof(double));
                *value = strtod(token->start_ptr, &token->end_ptr);
                vector_pushback(stack, value);
                break;
            case TokenTypeFunction:
                MathFunction *function = evaluator_get_function(evaluator, token);

                if (function == NULL) {
                    result.error_code = ErrorNoFunction;
                    return result;
                }

                bool success = function->callback(stack);
                if (!success) {
                    return result;
                }

                break;
            case TokenTypeUnaryOperator:
                MathOperator *operation = evaluator_get_unary_operator(evaluator, token);
                if (operation == NULL) {
                    result.error_code = ErrorNoUnaryOperator;
                    return result;
                }

                success = operation->callback(stack);
                if (!success) {
                    return result;
                }
                break;
            case TokenTypeOperator:
                operation = evaluator_get_operator(evaluator, token);
                if (operation == NULL) {
                    result.error_code = ErrorNoOperator;
                    return result;
                }

                success = operation->callback(stack);
                if (!success) {
                    return result;
                }

                break;
            case TokenTypeVariable:
                MathVariable *math_variable = evaluator_get_variable(evaluator, token);

                if (math_variable == NULL) {
                    result.error_code = ErrorNoVariable;
                    return result;
                }

                value = malloc(sizeof(double));
                *value = math_variable->value;
                vector_pushback(stack, value);
                break;

            default:
                break;
        }
    }

    double *evaluation_ptr = vector_pop(stack);
    result.result = *evaluation_ptr;
    result.success = true;

    free(evaluation_ptr);
    vector_free(stack);

    return result;
}

Evaluator evaluator_new() {
    Evaluator evaluator;

    evaluator.variables = vector_new();
    evaluator.operators = vector_new();
    evaluator.functions = vector_new();


    evaluator_create_operator(&evaluator, '+', 2, 2, true, &add_operator);
    evaluator_create_operator(&evaluator, '-', 2, 2, true, &subtract_operator);
    evaluator_create_operator(&evaluator, '*', 3, 2, true, &multiply_operator);
    evaluator_create_operator(&evaluator, '/', 3, 2, true, &divide_operator);
    evaluator_create_operator(&evaluator, '^', 5, 2, false, &power_operator);


    evaluator_create_unary_operator(&evaluator, '!', 6, true, &factorial_operator);
    evaluator_create_unary_operator(&evaluator, '-', 4, false, &negate_operator);
    evaluator_create_unary_operator(&evaluator, '+', 4, false, &positive_operator);

    evaluator_create_function(&evaluator, 1, "sqrt", &sqrt_function);
    evaluator_create_function(&evaluator, 1, "sin", &sin_function);
    evaluator_create_function(&evaluator, 1, "cos", &cos_function);
    evaluator_create_function(&evaluator, 1, "tan", &tan_function);
    evaluator_create_function(&evaluator, 1, "asin", &asin_function);
    evaluator_create_function(&evaluator, 1, "acos", &acos_function);
    evaluator_create_function(&evaluator, 1, "atan", &atan_function);
    evaluator_create_function(&evaluator, 1, "deg", &deg_function);
    evaluator_create_function(&evaluator, 1, "rad", &rad_function);

    evaluator_create_function(&evaluator, 1, "abs", &abs_function);
    evaluator_create_function(&evaluator, 1, "exp", &exp_function);
    evaluator_create_function(&evaluator, 1, "log", &log_function);
    evaluator_create_function(&evaluator, 1, "ln", &ln_function);

    evaluator_create_variable(&evaluator, "pi", M_PI);
    evaluator_create_variable(&evaluator, "e", M_E);


    return evaluator;
}

MathOperator *evaluator_get_operation(Evaluator *evaluator, const LexerToken *token, bool is_unary) {
    if (token->type != TokenTypeOperator) {
        if (token->type != TokenTypeUnaryOperator) {
            return NULL;
        }
    }

    for(int i = 0; i < evaluator->operators->length; i++) {
        MathOperator *operation = vector_get(evaluator->operators, i);

        if (operation->is_unary != is_unary) {
            continue;
        }

        if (operation->operator == *token->start_ptr) {
            return operation;
        }
    }

    return NULL;
}

MathOperator *evaluator_get_unary_operator(Evaluator *evaluator, const LexerToken *token) {
    return evaluator_get_operation(evaluator, token, true);
}
MathOperator *evaluator_get_operator(Evaluator *evaluator, const LexerToken *token) {
    return evaluator_get_operation(evaluator, token, false);
}

MathFunction *evaluator_get_function(Evaluator *evaluator, const LexerToken *token) {

    if (token->type != TokenTypeFunction) {
        return NULL;
    }

    for(int i = 0; i < evaluator->functions->length; i++) {
        MathFunction *function = vector_get(evaluator->functions, i);
        int token_length = token->end_ptr - token->start_ptr;

        if (strncmp(token->start_ptr, function->name, token_length) == 0) {
            return function;
        }
    }

    return NULL;
}

MathVariable *evaluator_get_variable(Evaluator *evaluator, const LexerToken *token) {

    if (token->type != TokenTypeVariable) {
        return NULL;
    }

    for(int i = 0; i < evaluator->variables->length; i++) {
        MathVariable *constant = vector_get(evaluator->variables, i);

        int token_length = token->end_ptr - token->start_ptr;

        if (strncmp(constant->name, token->start_ptr, token_length) == 0) {
            return constant;
        }
    }

    return NULL;
}

void evaluator_create_operator(
        Evaluator *evaluator,
        char operator,
        int precedence,
        int argument_count,
        bool left_associative,
        bool (*callback)(Vector *)) {
    MathOperator *operation = malloc(sizeof(MathOperator));
    operation->left_associative = left_associative;
    operation->precedence = precedence;
    operation->callback = callback;
    operation->is_unary = false;
    operation->is_important = false;
    operation->argument_count = argument_count;

    // +1 for the NULL terminator
    operation->operator = operator;

    vector_pushback(evaluator->operators, operation);
}

void evaluator_create_unary_operator(
        Evaluator *evaluator,
        const char operator,
        const int precedence,
        const bool is_important,
        bool (*callback)(Vector *)
) {
    MathOperator *operation = malloc(sizeof(MathOperator));
    operation->operator = operator;
    operation->precedence = precedence;
    operation->argument_count = 1;
    operation->is_unary = true;
    operation->is_important = is_important;
    operation->left_associative = false;
    operation->callback = callback;

    vector_pushback(evaluator->operators, operation);
}

void evaluator_create_function(Evaluator *evaluator, const int argument_count, const char *name, bool (*callback)(Vector *)) {
    int name_length = strlen(name);
    // +1 for the null terminator
    char *function_name = malloc(sizeof(char) * (name_length + 1));
    strcpy(function_name, name);

    MathFunction *function = malloc(sizeof(MathFunction));
    function->name = function_name;
    function->argument_count = argument_count;
    function->callback = callback;

    vector_pushback(evaluator->functions, function);
}

void evaluator_create_variable(Evaluator *evaluator, const char *name, double value) {
    MathVariable *constant = malloc(sizeof(MathVariable));
    constant->value = value;

    int name_length = strlen(name) + 1;
    constant->name = calloc(sizeof(char), name_length);
    strcpy(constant->name, name);

    vector_pushback(evaluator->variables, constant);
}

void evaluator_free(Evaluator *evaluator) {

    for(int i = 0; i < evaluator->variables->length; i++) {
        MathVariable *constant = vector_get(evaluator->variables, i);
        free(constant->name);
        free(constant);
    }

    for(int i = 0; i < evaluator->operators->length; i++) {
        MathOperator *operation = vector_get(evaluator->operators, i);
        free(operation);
    }

    for(int i = 0; i < evaluator->functions->length; i++) {
        MathFunction *operation = vector_get(evaluator->functions, i);
        free(operation->name);
        free(operation);
    }

    if (evaluator->variables != NULL) {
        vector_free(evaluator->variables);
    }

    if (evaluator->operators != NULL) {
        vector_free(evaluator->operators);
    }

    if (evaluator->functions != NULL) {
        vector_free(evaluator->functions);
    }
}

