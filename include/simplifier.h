#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include "evaluator.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    Vector *nodes;
    bool is_leaf;
    // is null if is_leaf is false
    LexerToken *token;
} ExpresionNode;


ExpresionNode *simplify_tree(ExpresionNode *node);
ExpresionNode *convert_to_tree(Vector *tokens, Evaluator *evaluator);
void flatten_tree(ExpresionNode *node);

void print_tree(ExpresionNode *node, int depth);

#endif
