#include "ast_tree.h"
#include "tests.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int main() {
    TestCase tests[] = {
        {"a", "a",},
        {"aa", "^(a,2)",},
        {"bada", "*(^(a,2),b,d)",},
        {"a+a+a-b+3a", "+(*(6,a),u_-(b))"},
        {"0*a+b", "b"},
        {"a^(-2)*a^5", "^(a,3)"},
        {"a+b+a", "+(*(2,a),b)",},
        {"a+b-a+c-b", "c"},
        {"bd + ba", "*(+(a,d),b)"},
        {"a - a", "0"},
        {"(c+d)^4*(c+d)^(b+a)", "^(+(c,d),+(4,a,b))"},
        {"2x-x", "x"},
        {"b^3/b^2", "b"},
        { "a^b^c","^(a,b,c)" },
        { "(a^b)^c","^(a,*(b,c))" },
        { "a^b^c / a^b^c","1" },
        { "a * a^b * a^c * a","^(a,+(2,b,c))" },
        { "(a+b)^2 / (a+b)","+(a,b)" },
        { "x^2 * x^3 * x^y","^(x,+(5,y))" },
        { "a + b + c - a - b - c","0" },
        { "x + x + y - x","+(x,y)" },
        { "3*a - a - a - a","0" },
        { "2*a - a - a","0" },
        { "a * (b + c) - a*b","*(a,c)" },
        { "(x+1)*(x-1)","+(^(x,2),u_-(1))" },
        { "a - (b - a)","+(*(2,a),u_-(b))" },
        { "((a^b)^c)^d + e^f^g","+(^(a,*(b,c,d)),^(e,f,g))" },
        { "x * x * x / (x^2 * x)","1" },
        { "a^2 + 2*a*b + b^2 - (a+b)^2","0" },
        { "x^2/x^2", "1"},
        {"ab/ab", "1"},
        { NULL, NULL }
    };

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

        free_ast(node);

        if (test_passed) {
            passed ++;
            print_as_passed(test_case);
        } else {
            failed++;
            print_as_failed(test_case, equation);
        }

        free(equation);
    }

    printf("passed %zu out of %zu\n", passed, passed+failed); 
}
