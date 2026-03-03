#include <stddef.h>
#include <stdio.h>
#include "ast_tree.h"
#include <time.h>
#include <stdlib.h>

void do_things(char *input) {
    AstNode *node = string_to_ast_node(input);
    print_ast_as_string(node);
    free_ast(node);
}

int main() {

    printf("Enter your equation: ");
    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);

    clock_t starting_clocks = clock();

    do_things(user_input);

    // do_things(user_input);
    free(user_input);
    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);

    printf("Simplified is: ");

    return 0;
}

