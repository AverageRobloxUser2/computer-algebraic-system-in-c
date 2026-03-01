#include <stdio.h>
#include "ast_tree.h"
#include <time.h>
#include <stdlib.h>

int main() {
    clock_t starting_clocks = clock();

    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);

    AstNode *node = string_to_ast_node(user_input);
    // print_ast_tree_as_graphviz(node);

    printf("ast node has %d children\n", node->child_count);
    print_ast_as_string(node);
    free(user_input);
    free_ast(node);

    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);

    return 0;
}

