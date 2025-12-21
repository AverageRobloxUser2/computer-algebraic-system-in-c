#ifndef SIMPLIFY_RULES_H
#define SIMPLIFY_RULES_H

#include "ast_tree.h"

typedef struct {
    char *left;
    char *right;
} MathSimplificationRule;

typedef struct {
    size_t variable_count;
    AstNode **_variable_values;
    // ast_node_to_string list
    char **_variable_value_strings;
    char **_variable_names;
} MathVariableMap;

MathVariableMap *create_variable_map(
    AstNode *node,
    AstNode *rule
);
MathVariableMap *create_variable_map(AstNode *node, AstNode *rule);
AstNode *variable_map_get_variable_value(MathVariableMap *map, char *name);
void free_variable_map(MathVariableMap *map);


MathVariableMap *does_node_match_rule(AstNode* node, AstNode *rule_left_node);
AstNode *create_node_form_rules(AstNode *rule, MathVariableMap *map);
#endif
