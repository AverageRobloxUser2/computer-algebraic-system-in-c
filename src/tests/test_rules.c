
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
        {"a/b*c/a", "c/b"},
        {"a-a", "0"},
        {"a-a+c", "c"},
        {"a+c-a", "c"},
        {NULL, NULL} // End of rules
    };

    // Example equation to simplify
    char *user_input = NULL;
    size_t line_length = 0;
    getline(&user_input, &line_length, stdin);
    // char *equation = "(x+4)(x-3) + (x-3)(x+7)";
    char *equation = user_input;
    // equation = "4*3/4";
    // Iterate through the defined rules and apply them
    for (size_t i = 0; rules[i].left != NULL; i++) {
        AstNode *node = string_to_ast_node(equation);
        MathSimplificationRule rule = rules[i];
        
        // Create AST nodes for the rule
        AstNode *rule_node = string_to_ast_node(rule.left);
        
        // Check if the current equation node matches the rule
        MathVariableMap *map = does_node_match_rule(node, rule_node);
        bool rules_match = (map != NULL);

        // Print results of rule application
        printf("%s -> %d\n", rule.left, rules_match);
        if (rules_match) {
            AstNode *right_node = string_to_ast_node(rule.right);
            printf("variable count: %d\n", map->variable_count);

            print_variables(map);
            AstNode *applied_rule = create_node_form_rules(right_node, map);
            ast_node_concat_operators(applied_rule);

            char *equation = ast_node_to_equation(applied_rule);
            printf("RESULT: %s\n", equation);

            free(equation);

            free_ast(right_node);
            free_ast(applied_rule);
        }

        // Clean up
        free_ast(rule_node);
        free_ast(node);
        // Make sure to free the variable map if it was allocated in does_node_match_rule
        if (map != NULL) {
            free_variable_map(map);
            break;
        }
    }

    free(user_input);
    
    return 0;
}
