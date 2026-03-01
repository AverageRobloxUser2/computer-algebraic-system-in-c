#include <stddef.h>
#include <stdio.h>
#include "ast_tree.h"
#include <time.h>
#include <stdlib.h>

void do_things(char *input) {
    AstNode *node = string_to_ast_node(input);
    printf("Simplified %s\ninto: ", input);
    print_ast_as_string(node);
    printf("------------\n");
    free_ast(node);
}

int main() {

    printf("Enter your equation: ");
    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);

    clock_t starting_clocks = clock();


    // do_things("a+a+a+a");                // Multi-term grouping
    // do_things("a-a-a-a");                // Unary minus vs Coefficients
    // do_things("ab+ac+ad");               // Multi-term factoring
    // do_things("a*1*b*1*d");              // Identity cleanup
    // do_things("a*a*b*b*a");              // Power reduction
    // do_things("a+b-a-b");                // Total collapse to zero
    // do_things("a*b*0");                  // Zero destruction
    do_things("a+(b+(c-c)-b)-a");        // Deeply nested cancellation
    // do_things("bad+bad+bad");            // Grouping complex products
                                         //

    do_things(user_input);
    free(user_input);
    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);

    printf("Simplified is: ");

    return 0;
}

