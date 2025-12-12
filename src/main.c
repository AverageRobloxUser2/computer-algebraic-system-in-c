#include "infix.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main() {
    clock_t starting_clocks = clock();

    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);


    LexerResult lexer_result = lex(user_input);
    InfixEquation infix = convert_lexed_to_infix(lexer_result);
    free(user_input);
    free(lexer_result.tokens);

    for(int i = 0; i < infix.token_count; i++) {
        printf("%s", infix.tokens[i].value);
    }
    printf("\n");

    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);
}
