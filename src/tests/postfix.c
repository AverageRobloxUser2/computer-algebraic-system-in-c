#include "postfix.h"
#include "infix.h"
#include "lexer.h"
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
    PostfixEquation postfix = convert_infix_to_postfix(infix);


    for(int i = 0; i < lexer_result.token_count; i++) {
        free(lexer_result.tokens[i].value);
    }
    free(lexer_result.tokens);

    char *result = NULL;
    int result_length = 0;

    for(size_t i = 0; i < postfix.token_count; i++) {
        char *value = postfix.tokens[i].value;

        result = reallocarray(
            result,
            result_length + strlen(value) + 1,
            sizeof(char)
        );

        strcpy(result + result_length, value);
        result_length = strlen(result);
        free(value);
    }

    return result;
}

int main() {


    EquationToInfixTest infixTests[] = {
        {"x+y", "xy+"},
        {"x+y+z", "xy+z+"},
        {NULL, NULL},
    };

    size_t i = 0;
    size_t passed_count = 0;
    size_t failed = 0;
    while(true) {
        EquationToInfixTest test = infixTests[i];
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
