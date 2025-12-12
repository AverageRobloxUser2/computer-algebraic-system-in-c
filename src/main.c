#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "evaluator.h"
#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include "parser.h"
#include <string.h>

EvaluationResult calculate_answer(char *user_input) {
    // i do because when user_input is provided with args as a string
    // called like this calculate_answer("2 + 2") it errors
    int input_length = strlen(user_input);
    char *input = malloc(sizeof(char) * (input_length + 1));
    strcpy(input, user_input);

    Evaluator evaluator = evaluator_new();

    EvaluationResult result;
    result.success = false;


    Vector *tokens = evaluator_tokenize(&evaluator, input);

    if (!tokens) {
        return result;
    }

    Vector *rpn_tokens = parse(tokens, &evaluator);

    for(int i = 0; i < rpn_tokens->length; i++) {
        LexerToken *token = vector_get(rpn_tokens, i);
        printf("%c ", *token->start_ptr);
    }
    printf("\n");

    result = evaluate(&evaluator, rpn_tokens);

    ExpresionNode *node = convert_rpn_tokens_to_tree(rpn_tokens, &evaluator);
    print_tree(node, 1);
    printf("flattening root\n");
    flatten_tree(node);
    print_tree(node, 1);
    printf("compacting root\n");
    compact_operators(node);
    print_tree(node, 1);
    for(int i = 0; i < tokens->length; i++) {
        LexerToken *token = vector_get(tokens, i);
        free(token);
    }


    vector_free(tokens);
    vector_free(rpn_tokens);
    evaluator_free(&evaluator);

    return result;
}

int main() {
    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);
    // READ THIS YOU FOOL
    // 1. allocates the result "tokens" variable
    // 2. in tokens allocates tokens._values
    // 3. these both are handled by vector_free(tokens)
    // 4. each entry inside tokens is also allocated 
    // 5. entry is allocated and entry->value is also
    //      so free them dumbass
    
    clock_t starting_clocks = clock();

    EvaluationResult result;
    result = calculate_answer(user_input);

    printf("Answer: %lf\n", result.result);
    printf("Success: %d\n", result.success);
    free(user_input);
}
