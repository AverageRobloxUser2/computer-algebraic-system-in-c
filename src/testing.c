#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "evaluator.h"
#include "tokenizer.h"
#include "vector.h"
#include "parser.h"
#include <string.h>

EvaluationResult calculate_answer(char *user_input) {
    int input_length = strlen(user_input);
    char *input = malloc(sizeof(char) * (input_length + 1));
    strcpy(input, user_input);
    Evaluator evaluator = evaluator_new();

    EvaluationResult result;
    result.success = false;

    Vector *tokens = evaluator_tokenize(&evaluator, input);

    if (!tokens) {
        return result;
    }

    Vector *rpn_tokens = parse(tokens, &evaluator);

    result = evaluate(&evaluator, rpn_tokens);

    for(int i = 0; i < tokens->length; i++) {
        LexerToken *token = vector_get(tokens, i);
        free(token);
    }



    vector_free(tokens);
    vector_free(rpn_tokens);
    evaluator_free(&evaluator);

    free(input);


    return result;
}

typedef struct {
    char *equation;
    double answer;
} TestingCase;

int main() {
    TestingCase equations[] = {
        // Basic arithmetic operations
        {"2 + 3", 2 + 3}, // Addition
        {"5 - 2", 5 - 2}, // Subtraction
        {"4 * 3", 4 * 3}, // Multiplication
        {"8 / 2", 8 / 2}, // Division
        {"2 ^ 3", pow(2, 3)}, // Exponentiation

        // Unary operations
        {"5!", 120}, // Factorial
        {"-3", -3}, // Negation
        {"+4", +4}, // Positive

        // Functions
        {"sqrt(16)", sqrt(16)}, // Square root
        {"sin(pi/2)", sin(M_PI / 2)}, // Sine
        {"cos(0)", cos(0)}, // Cosine
        {"tan(pi/4)", tan(M_PI / 4)}, // Tangent
        {"asin(0.5)", asin(0.5)}, // Inverse sine
        {"acos(0.5)", acos(0.5)}, // Inverse cosine
        {"atan(1)", atan(1)}, // Inverse tangent
        {"abs(-5)", abs(-5)}, // Absolute value
        {"exp(1)", exp(1)}, // Exponential
        {"log(10)", log10(10)}, // Logarithm base e
        {"ln(e)", log(M_E)}, // Natural logarithm
        {"rad(45)", M_PI * 45 / 180}, // Convert to radians
        {"deg(rad(45))", 45}, // convert to degress

        // Constants
        {"pi", M_PI}, // Pi constant
        {"e", M_E}, // Pi constant

        // Mixed operations
        {"2 + 3 * 4", 2 + 3 * 4}, // Mixed addition and multiplication
        {"(2 + 3) * 4", (2 + 3) * 4}, // Parentheses
        {"2 * (3 + 4)", 2 * (3 + 4)}, // Parentheses with addition

        // Implicit multiplication
        {"2(3)", 2 * 3}, // Implicit multiplication
        {"(2 + 3)(4)", (2 + 3) * 4}, // Implicit multiplication with parentheses
        {"2sin(4)", 2 * sin(4)}, // Implicit multiplication with parentheses

        // Edge cases
        {"0 * 5", 0 * 5}, // Multiplication by zero
        {"5(0)", 5 * 0}, // Implicit multiplication by zero
        {"(5)(0)", 5 * 0}, // Implicit multiplication by zero

        // Weird but valid expressions
        {"(2 + 3)!(2)", 120 * 2}, // Factorial with addition
        {"2--2", 2 - (-2)}, // Double negation
        {"2++2", 2 + 2}, // Double addition
        {"(4)((sin(asin(0.5))))", 4 * sin(asin(0.5))}, // Nested functions with implicit multiplication
        {"(2 + 3)(4 + 1)", (2 + 3) * (4 + 1)}, // Implicit multiplication with sums
        {"(sin(0) + cos(0))(5)", (sin(0) + cos(0)) * 5}, // Implicit multiplication with sum of functions
        {"5!2", 120 * 2}, // Factorial with addition
        {"(tan(pi/4))(2)", tan(M_PI / 4) * 2}, // Implicit multiplication with tangent
        {"(cos(pi/3))(3)", cos(M_PI / 3) * 3}, // Implicit multiplication with cosine

        // Null case
        {NULL, 0.0},
    };

    int i = -1;
    while(true) {
        i++;

        TestingCase testing_shit = equations[i];
        if (testing_shit.equation == NULL) {
            break;
        }

        char *equation = testing_shit.equation;

        EvaluationResult result = calculate_answer(testing_shit.equation);

        printf("%d. %s: ", i, equation);

        if (!result.success) {
            printf("\e[1;31m: failed (%d)\n\e[0m", equation, result.error_code);
            continue;
        }

        double expected_answer = testing_shit.answer;
        double answer_difference = result.result - expected_answer;
        if (pow(answer_difference, 2) > 0.01) {
            printf(
                "\e[1;31mwrong answer\n\t(got, expected): %lf, %lf\e[0m\n",
                result.result, expected_answer);
            continue;
        }
        printf("\e[1;32mcorrect answer\e[0m\n");
    }

}
