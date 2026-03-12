#ifndef AST_TREE_H
#define AST_TREE_H
#include "math_equation.h"
#include "postfix.h"
#include <stddef.h>

typedef struct AstNode_S AstNode;

struct AstNode_S {
    MathEquationTokenType type;

    AstNode **children_ptrs;
    size_t child_count;

    double value;
    char *name;
};


AstNode *create_new_node(
    MathEquationTokenType type,
    char *name);

AstNode *create_number_node(double number);

AstNode *deep_clone_node(
    AstNode *node
);

void sort_node(
    AstNode *node
);

void append_child_node(AstNode *parent, AstNode *node);

void remove_and_free_child_at_index(AstNode *parent, size_t index);
void remove_and_free_child(AstNode *parent, AstNode *child);


void replace_node_with_another(AstNode *node, AstNode *replacment);

int find_child_index(AstNode *parent, AstNode *child);

AstNode *postfix_to_ast(PostfixEquation postfix);
AstNode *string_to_ast_node(char *input);

void free_ast(AstNode *node);
void ast_shallow_free(AstNode *node);
void ast_free_children(AstNode *node);

void print_ast_tree_as_graphviz(AstNode *node);
void print_ast_as_string(AstNode *node);
void print_ast_as_equation(AstNode *node);

char *ast_node_to_string(AstNode* node);
char *ast_node_to_equation(AstNode *node);

bool ast_node_is_same_node(AstNode *node_a, AstNode *node_b);
bool ast_node_only_contains_numbers(AstNode *node);
bool ast_node_matches_requirements(
    AstNode *node,
    MathEquationTokenType type, 
    char *name
);


// simplifying

bool ast_node_concat_operators(AstNode *node); 
bool ast_node_concated_power_into_multiplication(AstNode *node);
bool ast_node_expand_unary(AstNode *node);
bool ast_node_conditionaly_expand_power(AstNode *node);
bool ast_node_subtraction_into_negated_addition(AstNode *node);
bool ast_node_division_into_multiplication(AstNode *node);
bool ast_node_expand_multiplicated_power(AstNode *node);
bool ast_node_expand_multipcation(AstNode *node);
bool ast_node_constant_fold(AstNode *node);
bool ast_node_expand_negated_addition(AstNode *node);

bool ast_node_simplify_multiplication_convert_to_power(AstNode *node);
bool ast_node_simplify_addition_convert_to_multiplication(AstNode *node);
bool ast_node_simplify_same_multiplicator_addition(AstNode *node);
bool ast_node_simplify_multipliaction_by_1(AstNode *node);
bool ast_node_simplify_double_unary(AstNode *node);
bool ast_node_simplify_power_identities(AstNode *node);
bool ast_node_simplify_addition_with_fractions(AstNode *node);


#endif
