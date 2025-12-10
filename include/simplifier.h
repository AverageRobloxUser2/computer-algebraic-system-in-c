#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include "evaluator.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    // is NULL is is_leaf is true
    Vector *nodes;
    bool is_leaf;
    enum LexerTokenType type;

    // is a string with a single value for operators
    char *name;
    // is 0 for everything not a TokenTypeNumber
    double value;
} ExpresionNode;



ExpresionNode *convert_to_tree(Vector *tokens, Evaluator *evaluator);

void *simplify_tree(ExpresionNode *node);
void flatten_tree(ExpresionNode *node);
void compact_operators(ExpresionNode *node);

void print_tree(ExpresionNode *node, int depth);

#endif
