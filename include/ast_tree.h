#ifndef AST_TREE_H
#define AST_TREE_H
#include "math_equation.h"
#include "postfix.h"

typedef struct AstNode_S AstNode;

struct AstNode_S {
    MathEquationTokenType type;

    AstNode **children_ptrs;
    size_t child_count;

    double value;
    char *name;
};

void append_child_node(AstNode *parent, AstNode *node);
AstNode *postfix_to_ast(PostfixEquation postfix);
void free_ast(AstNode *);
void print_ast_tree_as_graphviz(AstNode *node);

#endif
