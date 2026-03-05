#include <stddef.h>
#include <stdio.h>
#include "ast_tree.h"
#include <time.h>
#include <stdlib.h>

void do_things(char *input) {
    AstNode *node = string_to_ast_node(input);
    // print_ast_as_string(node);
    char *eq = ast_node_to_equation(node);
    printf("%s -> %s\n", input, eq);
    print_ast_as_string(node);
    free(eq);
    free_ast(node);
}

int main() {
    printf("Enter your equation: ");
    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);

    clock_t starting_clocks = clock();

    do_things(user_input);
    free(user_input);
    // --- Round 1: Power Tower vs. Product (The Logic Test) ---
    // do_things("a^b^c");           // Expected: ^(a, b, c) [Right-associative tower]
    // do_things("(a^b)^c");         // Expected: ^(a, *(b, c)) [Power-of-a-power rule]
    // do_things("a^b^c / a^b^c");   // Expected: 1 (or 0 as exponent) [Structural Identity]
    //
    // // --- Round 2: Exponent Collection (The Exponent Factor Test) ---
    // do_things("a * a^b * a^c * a"); // Expected: ^(a, +(b, c, 2)) [Implicit 1s collected]
    // do_things("(a+b)^2 / (a+b)");   // Expected: +(a, b) [Base matching with complex nodes]
    // do_things("x^2 * x^3 * x^y");   // Expected: ^(x, +(y, 5)) [Constant folding + Variable]
    //
    // // --- Round 3: Additive Simplification (The "Zero" Test) ---
    // do_things("a + b + c - a - b - c"); // Expected: 0 [Selection Sort + Cancellation]
    // do_things("x + x + y - x");         // Expected: +(x, y) [Term collection]
    // do_things("3*a - a - a - a");       // Expected: 0 [Coefficient math]
    //
    // do_things("2a-a-a");
    // // --- Round 4: Distribution & Expansion (The Expansion Strategy) ---
    // // do_things("a * (b + c) - a*b");     // Expected: a*c [Distribution + Subtraction]
    // // do_things("(x+1)*(x-1)");           // Expected: x^2 - 1 [FOIL / Difference of squares]
    // do_things("a - (b - a)");           // Expected: 2*a - b [Unary minus distribution]
    //
    // // --- Round 5: The "Boss Fight" (Total Structural Integrity) ---
    // do_things("((a^b)^c)^d + e^f^g");             // Expected: +( ^(a, *(b,c,d)), ^(e,f,g) )
    // do_things("x * x * x / (x^2 * x)");           // Expected: 1
    // // do_things("a^2 + 2*a*b + b^2 - (a+b)^2");     // Expected: 0 [The ultimate expansion test]

    //
    // do_things(user_input);
    //
    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);

    return 0;
}

