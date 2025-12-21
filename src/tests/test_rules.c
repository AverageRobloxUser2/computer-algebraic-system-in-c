
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ast_tree.h"
#include "rules.h"

void print_variables(MathVariableMap *map) {
    for(size_t i = 0; i < map->variable_count; i++) {
        printf(
            "%s -> %s\n",
            map->_variable_names[i],
            map->_variable_value_strings[i]
        );
    }
}

int main() {
    // Define simplification rules
    MathSimplificationRule rules[] = {
        {"ab+cb", "b*(a+c)"}, 
        {"ab+bc", "b*(a+c)"}, 
        {"ba+bc", "b*(a+c)"}, 
        {"--a", "a"},
        {"1*a", "a"},
        {"a/1", "a"},
        {"a/a", "1"},
        {"a*b/a", "b"},
        {NULL, NULL} // End of rules
    };

    // Example equation to simplify
    char *equation = "(x+4)(x-3) + (x-3)(x+7)";
    // equation = "4*3/4";
    AstNode *node = string_to_ast_node(equation);
    ast_node_concat_operators(node);

    print_ast_tree_as_graphviz(node);

    // Iterate through the defined rules and apply them
    for (size_t i = 0; rules[i].left != NULL; i++) {
        MathSimplificationRule rule = rules[i];
        
        // Create AST nodes for the rule
        AstNode *rule_node = string_to_ast_node(rule.left);
        AstNode *left_node = string_to_ast_node(rule.right);
        
        // Check if the current equation node matches the rule
        MathVariableMap *map = does_node_match_rule(node, rule_node);
        bool rules_match = (map != NULL);

        // Print results of rule application
        printf("%s -> %d\n", rule.left, rules_match);
        if (rules_match) {
            printf("variable count: %d\n", map->variable_count);
            print_variables(map);
            AstNode *applied_rule = create_node_form_rules(left_node, map);
            ast_node_concat_operators(applied_rule);
            printf("RESULT: ");
            print_ast_tree_as_graphviz(applied_rule);
            free(applied_rule->children_ptrs);
            free(applied_rule->name);
            free(applied_rule);
            // free_ast(applied_rule);
        }

        // Clean up
        free_ast(rule_node);
        free_ast(left_node);
        // Make sure to free the variable map if it was allocated in does_node_match_rule
        if (map != NULL) {
            free_variable_map(map);
        }
    }

    // Clean up the original equation AST
    free_ast(node);
    
    return 0;
}
