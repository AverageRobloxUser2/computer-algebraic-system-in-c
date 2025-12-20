#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ast_tree.h"

int main() {
    clock_t starting_clocks = clock();

    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);


    LexerResult lexer_result = lex(user_input);
    free(user_input);

    InfixEquation infix = convert_lexed_to_infix(lexer_result);
    free_lexed(lexer_result);

    PostfixEquation postfix = convert_infix_to_postfix(infix);
    free_equation(infix);

    for(size_t i = 0; i < postfix.token_count; i++) {
        printf("%s", postfix.tokens[i].value);
    }

    printf("\n");

    AstNode *node = postfix_to_ast(postfix);
    free_equation(postfix);

    // print_ast_tree_as_graphviz(node);
    char *node_before_concating = ast_node_to_string(node);
    printf("node as string: %s\n", node_before_concating);
    ast_node_concat_operators(node);
    char *node_after_concating = ast_node_to_string(node);
    printf("node as string: %s\n", node_after_concating);
    free(node_before_concating);
    free(node_after_concating);
    free_ast(node);

    clock_t ending_clocks = clock();
    clock_t delta_clocks = ending_clocks - starting_clocks;
    printf("Done in: %lf\n", (double)delta_clocks / CLOCKS_PER_SEC);

    return 0;
}
