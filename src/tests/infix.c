#include "infix.h"
#include "lexer.h"
#include "math_equation.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char *input;
    char *output;
} EquationToInfixTest;

char *to_infix(char *input) {
    char *input_cloned = calloc(strlen(input) + 1, sizeof(char));
    strcpy(input_cloned, input);

    LexerResult lexer_result = lex(input_cloned);
    free(input_cloned);
    InfixEquation infix = convert_lexed_to_infix(lexer_result);


    for(int i = 0; i < lexer_result.token_count; i++) {
        free(lexer_result.tokens[i].value);
    }
    free(lexer_result.tokens);

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

    free_equation(infix);

    return result;
}

int main() {


    EquationToInfixTest infix_tests[] = {
        {"3x+4y", "3*x+4*y"},
        {"5(a+2)", "5*(a+2)"},
        {"2xy+3z", "2*x*y+3*z"},
        {"x^2y", "x^2*y"},
        {"4(3x+2)", "4*(3*x+2)"},
        {"(x+2)(y+3)", "(x+2)*(y+3)"},
        {"3x/2y", "3*x/(2*y)"},
        {"2!2", "2!*2"},
        {"0", "0"},
        {"x+y+z", "x+y+z"},
        {"3+4", "3+4"},
        {"1/4", "1/(4)"},
        {"(x+1)+(y+2)", "(x+1)+(y+2)"},
        {"x^3", "x^3"},
        {"1/x+4", "1/(x)+4"},
        {"-2(x+3)", "-2*(x+3)"},
        {"(-2)(x+3)", "(-2)*(x+3)"},
        {"-2/(xy+3)", "-2/((x*y)+3)"},
        {NULL, NULL},
    };

    size_t i = 0;
    size_t passed_count = 0;
    size_t failed = 0;
    while(true) {
        EquationToInfixTest test = infix_tests[i];
        if(test.input == NULL) {
            break;
        }
        char *got_output = to_infix(test.input);
        char *expected_output = test.output;
        bool passed = strcmp(got_output, expected_output) == 0;
        if (passed) {
            printf(
                ANSI_COLOR_GREEN "%s -> %s\n" ANSI_COLOR_RESET,
                test.input,
                got_output
            );
            passed_count++;
        } else {
            printf(
                ANSI_COLOR_RED "FAILED: %s EXPECTED %s got -> %s\n" ANSI_COLOR_RESET,
                test.input,
                expected_output,
                got_output
            );
            failed++;
        }


        free(got_output);
        i++;
    }

    printf("-------------\n");
    printf("STATS:\n");
    printf("passed: %ld\n", passed_count);
    printf("failed: %ld\n", failed);
}
