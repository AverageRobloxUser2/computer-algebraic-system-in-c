#include "ast_tree.h"
#include "tests.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "simplficiation_tests.h"

int main() {
    clock_t starting_clocks = clock();

    size_t i = 0;

    size_t failed = 0;
    size_t passed = 0;
    while(true) {
        TestCase test_case = tests[i];
        if (test_case.input == NULL) {
            break;
        }
        i++;

        AstNode *node = string_to_ast_node(test_case.input);
        char *equation = ast_node_to_string(node);

        bool test_passed = strcmp(test_case.output, equation) == 0;


        if (test_passed) {
            passed ++;
            print_as_passed(test_case);
        } else {
            failed++;
            print_as_failed(test_case, equation);
        }

        free(equation);

        free_ast(node);
    }

    printf("Passed %zu out of %zu\n", passed, passed+failed); 
    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("%lfs\n", (double)delta_clocks / CLOCKS_PER_SEC);
}
