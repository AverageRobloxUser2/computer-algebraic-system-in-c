#include "ast_tree.h"
#include "tests.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int main() {
    TestCase tests[] = {
        { "a", "a",},
        { "aa", "^(a,2)",},
        { "bada", "*(^(a,2),b,d)",},
        { "a+b+a", "+(*(2,a),b)",},
        { NULL, NULL }
    };

    size_t i = 0;
    while(true) {
        TestCase test_case = tests[i];
        if (test_case.input == NULL) {
            break;
        }
        i++;

        AstNode *node = string_to_ast_node(test_case.input);
        char *equation = ast_node_to_string(node);

        bool test_passed = strcmp(test_case.output, equation) == 0;

        free_ast(node);

        if (test_passed) {
            print_as_passed(test_case);
        } else {
            print_as_failed(test_case, equation);
        }
        free(equation);
    }
}
