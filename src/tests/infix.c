#include "infix.h"
#include "lexer.h"
#include "math_equation.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char *input;
    char *output;
    MathEquationTokenType tokens[100];
} EquationToInfixTest;

char *infix_to_string(InfixEquation infix) {
    char *result = NULL;
    int result_length = 0;

    for(size_t i = 0; i < infix.token_count; i++) {
        char *value = infix.tokens[i].value;

        result = reallocarray(
            result,
            result_length + strlen(value) + 1,
            sizeof(char)
        );

        strcpy(result + result_length, value);
        result_length = strlen(result);
    }
    
    return result;
}

InfixEquation to_infix(char *input) {
    char *input_cloned = calloc(strlen(input) + 1, sizeof(char));
    strcpy(input_cloned, input);

    LexerResult lexer_result = lex(input_cloned);
    free(input_cloned);
    InfixEquation infix = convert_lexed_to_infix(lexer_result);
    free_lexed(lexer_result);

    return infix;
}


bool passed_string_test(InfixEquation infix, EquationToInfixTest test) {
    char *got_output = infix_to_string(infix);
    char *expected_output = test.output;
    bool passed_string_output = strcmp(got_output, expected_output) == 0;

    if (!passed_string_output) {
        printf("--------------\n");
        printf("got %s expected %s\n", got_output, expected_output);
    }

    free(got_output);

    return passed_string_output;
}

bool passed_type_test(InfixEquation infix, EquationToInfixTest test) {
    bool passed = true;
    for(size_t i = 0; i < infix.token_count; i++) {
        MathEquationToken token = infix.tokens[i];
        MathEquationTokenType expected_type = test.tokens[i];
        if (expected_type == MathWrongToken) {
            printf("--------------\n");
            printf("Length of test and result token types doesnt match\n");
            passed = false;
            break;
        }

        if (token.type != expected_type) {
            printf("--------------\n");
            printf("passed the string test tho\n");
            printf("failed on token: %s\n", token.value);
            printf("Expected token type %u got %u\n", expected_type, token.type);
            passed = false;
            break;
        }
    }

    return passed;
}

int main() {

EquationToInfixTest infix_tests[] = {
    {
        "3x+4y",
        "3*x+4*y",
        {
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathWrongToken,
        },
    },
    {
        "a--b",
        "a--b",
        {
            MathVariableToken,
            MathOperatorToken,
            MathUnaryOperatorToken,
            MathVariableToken,
            MathWrongToken,
        },
    },
    {
        "5(a+2)",
        "5*(a+2)",
        {
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "2xy+3z",
        "2*x*y+3*z",
        {
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathWrongToken,
        },
    },
    {
        "x^2y",
        "x^2*y",
        {
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathWrongToken,
        },
    },
    {
        "4(3x+2)",
        "4*(3*x+2)",
        {
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "(x+2)(y+3)",
        "(x+2)*(y+3)",
        {
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "3x/2y",
        "3*x/(2*y)",
        {
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathNumberToken,
            MathOperatorToken,
            MathVariableToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "2!2",
        "2!*2",
        {
            MathNumberToken,
            MathOperatorToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "0",
        "0",
        {
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "x+y+z",
        "x+y+z",
        {
            MathVariableToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathVariableToken,
            MathWrongToken,
        },
    },
    {
        "3+4",
        "3+4",
        {
            MathNumberToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "1/4",
        "1/(4)",
        {
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "(x+1)+(y+2)",
        "(x+1)+(y+2)",
        {
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "x^3",
        "x^3",
        {
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "1/x+4",
        "1/(x)+4",
        {
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathParenthasisClosedToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "-2(x+3)",
        "-2*(x+3)",
        {
            MathUnaryOperatorToken,
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "(-2)(x+3)",
        "(-2)*(x+3)",
        {
            MathParenthasisOpenToken,
            MathUnaryOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "-2/(xy+3)",
        "-2/((x*y)+3)",
        {
            MathUnaryOperatorToken,
            MathNumberToken,
            MathOperatorToken,
            MathParenthasisOpenToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathOperatorToken,
            MathVariableToken,
            MathParenthasisClosedToken,
            MathOperatorToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "f(x)",
        "f(x)",
        {
            MathFunctionToken,
            MathParenthasisOpenToken,
            MathVariableToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "_xy+1",
        "_xy+1",
        {
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {
        "_xy(1)",
        "_xy(1)",
        {
            MathFunctionToken,
            MathParenthasisOpenToken,
            MathNumberToken,
            MathParenthasisClosedToken,
            MathWrongToken,
        },
    },
    {
        "xy+1",
        "x*y+1",
        {
            MathVariableToken,
            MathOperatorToken,
            MathVariableToken,
            MathOperatorToken,
            MathNumberToken,
            MathWrongToken,
        },
    },
    {NULL, NULL, {MathWrongToken}},
};

    size_t i = 0;
    size_t passed_count = 0;
    size_t failed = 0;
    while(true) {
        EquationToInfixTest test = infix_tests[i];
        if(test.input == NULL) {
            break;
        }
        InfixEquation infix = to_infix(test.input);
        if (passed_string_test(infix, test) && passed_type_test(infix, test)) {
            printf(
                ANSI_COLOR_GREEN "%s\n" ANSI_COLOR_RESET,
                test.input
            );
            passed_count++;
        } else {
            printf(
                ANSI_COLOR_RED "FAILED: %s\n" ANSI_COLOR_RESET,
                test.input
            );
            printf("--------------\n");
            failed++;
        }
        free_equation(infix);
        i++; 
    }

    printf("-------------\n");
    printf("STATS:\n");
    printf("passed: %ld\n", passed_count);
    printf("failed: %ld\n", failed);
}
