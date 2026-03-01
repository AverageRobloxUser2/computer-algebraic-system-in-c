
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ast_tree.h"
#include "rules.h"

void print_variables(MathVariableMap *map) {
    return;
    for(size_t i = 0; i < map->variable_count; i++) {
        printf(
            "%s -> %s\n",
            map->_variable_names[i],
            map->_variable_value_strings[i]
        );
    }
}

AstNode *try_apply_rule(AstNode *node, AstNode *rule_left, AstNode *rule_right) {
    // Check if the current equation node matches the rule
    MathVariableMap *map = does_node_match_rule(node, rule_left);
    bool rules_match = (map != NULL);
    if (rules_match) {
        AstNode *applied_rule = create_node_form_rules(rule_right, map);
        ast_node_concat_operators(applied_rule);
        return applied_rule;
    } else {
        AstNode *new_node = deep_clone_node(node);
        bool applied_a_rule = false;
        for(size_t i = 0; i < new_node->child_count; i++) {
            AstNode *applied_rule_node = try_apply_rule(
                    new_node->children_ptrs[i],
                    rule_left,
                    rule_right
            );

            if (applied_rule_node == NULL) {
                continue;
            }

            free_ast(new_node->children_ptrs[i]);
            new_node->children_ptrs[i] = applied_rule_node;
            applied_a_rule = true;
        }

        if (applied_a_rule) {
            return new_node;
        }

        free_ast(new_node);
    }

    return NULL;
}

AstNode *try_apply_rules(MathSimplificationRule *rules, AstNode *node) {
    for (size_t i = 0; rules[i].left != NULL; i++) {
        MathSimplificationRule rule = rules[i];
        
        // Create AST nodes for the rule
        AstNode *rule_left = string_to_ast_node(rule.left);
        AstNode *rule_right = string_to_ast_node(rule.right);

        AstNode *applied = try_apply_rule(node, rule_left, rule_right);
        if (applied == NULL) {
            continue;
        }

        printf("MATCHED RULE: %s\n", rule.left);
        return applied;
    }
    
    return NULL;
}

int main() {
    // Define simplification rules

    MathSimplificationRule rules[] = {
        // Distributive Property (Factorization)
        // Identity Rules

        // Basic Power Rules (assuming ^ notation)
        {"a * a",   "a^2"},
        {"a * a * b",   "a^2 * b"},
        {"a * b * a",   "a^2 * b"},
        {"b * a * a",   "a^2 * b"},
        {"a^n * a",   "a^(n+1)"},
        {"a^1",   "a"},
        {"a^0",   "1"},
        {"a^n^b",   "a^(n*b)"},

        // Negation and Subtraction
        {"a-0",   "a"},
        {"a-a",   "0"},
        {"--a",   "a"},
        {"a+(-b)", "a-b"},

        {NULL, NULL} // End of rules
    };

    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);
    AstNode *node = string_to_ast_node(user_input);

    print_ast_as_string(node);

    AstNode *previous = NULL;
    AstNode *applied = deep_clone_node(node);

    applied = try_apply_rules(rules, applied);

    char *equation = ast_node_to_equation(previous);
    // print_ast_as_string(previous);

    printf("RES: ---------\n");
    printf("RES: %s", user_input);
    printf("RES: %s\n", equation);
    free(equation);
    free(user_input);
    free_ast(node);
    free_ast(previous);
    
    return 0;
}
