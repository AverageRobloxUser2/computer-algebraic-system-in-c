#include "ast_tree.h"
#include "math_equation.h"
#include "rules.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int math_variable_map_get_name_index(MathVariableMap *map, char *name) {
    // returns -1 if index not fond
    for(size_t i = 0; i < map->variable_count; i++) {
        char *map_name = map->_variable_names[i];

        if (strcmp(map_name, name) == 0) {
            return i;
        }
    }

    return -1;
}

bool math_variable_map_match_variable(MathVariableMap *map, size_t index, AstNode *node) {
    if (index == -1u) {
        return false;
    }

    char *string_value = ast_node_to_string(node);

    char *old_value = map->_variable_value_strings[index];
    bool matches = strcmp(string_value, old_value) == 0;
    printf("comparing \"%s\" and \"%s\"\n", string_value, old_value);
    printf("result %d\n", matches);

    free(string_value);

    return matches;
}

void math_variable_map_insert(MathVariableMap *map, char *name, AstNode *node) {
    map->variable_count++;
    map->_variable_names = reallocarray(
        map->_variable_names,
        map->variable_count,
        sizeof(char*)
    );
    
    map->_variable_value_strings = reallocarray(
        map->_variable_value_strings,
        map->variable_count,
        sizeof(char*)
    );

    map->_variable_values = reallocarray(
        map->_variable_values,
        map->variable_count,
        sizeof(AstNode*)
    );

    size_t last = map->variable_count-1;
    map->_variable_value_strings[last] = ast_node_to_string(node);
    map->_variable_values[last] = node;
    map->_variable_names[last] = name;
    printf("INSERT %s AS ", name);
    print_ast_as_string(node);
}

bool populate_variable_map(MathVariableMap *map, AstNode *node, AstNode *rule) {
    if (node->child_count != rule->child_count) {
        printf("child count not equal %ld!=%ld\n", node->child_count, rule->child_count);
        printf("node: ");
        print_ast_as_string(node);
        printf("rule: ");
        print_ast_as_string(rule);
        return false;
    }

    if (node->type != rule->type) {
        printf("types not equal %d!=%d\n", node->type, rule->type);
        return false;
    }

    if (strcmp(node->name, rule->name)) {
        printf("names not equal %s and %s\n", node->name, rule->name);
        return false;
    }

    for(size_t i = 0; i < rule->child_count; i++) {
        AstNode *rule_child = rule->children_ptrs[i];
        AstNode *child = node->children_ptrs[i];

        if (rule_child->type == MathVariableToken) {
            int variable_index = math_variable_map_get_name_index(
                map,
                rule_child->name
            );

            bool map_contain_variable = variable_index >= 0;

            if (!map_contain_variable) {
                math_variable_map_insert(
                    map,
                    rule_child->name,
                    child
                );
                continue;
            }

            bool variable_in_map_same_as_this_one = math_variable_map_match_variable(
                map,
                variable_index,
                child
            );

            // we already have registered this node in the map, so we skip
            if (variable_in_map_same_as_this_one) {
                printf("variable %s already registered\n", rule_child->name);
                continue;
            }

            printf("we found a variable that should be of the same value\n");
            printf("node: ");
            print_ast_as_string(child);
            printf("rule: ");
            print_ast_as_string(rule_child);
            // we found a variable that should be of the same value but is not
            // for example rule (a-a) and node is (c-b)
            // this will get called on b since the map contains
            // the name a, but the value of it is "c" not "b"
            return false;
        } else {
            bool success = populate_variable_map(map, child, rule_child);
            if (!success) {
                return success;
            }
        }
    }

    return true;
}

void free_variable_map(MathVariableMap *map) {
    for(size_t i = 0; i < map->variable_count; i++) {
        free(map->_variable_value_strings[i]);
    }
    map->variable_count = 0;
    if (map->_variable_value_strings != NULL) {
        free(map->_variable_value_strings);
        map->_variable_value_strings = NULL;
    }

    if (map->_variable_names != NULL) {
        free(map->_variable_names);
        map->_variable_names = NULL;
    }
    if (map->_variable_values != NULL) {
        free(map->_variable_values);
        map->_variable_values = NULL;
    }

    free(map);
}

MathVariableMap *create_variable_map(AstNode *node, AstNode *rule) {
    if (node->child_count != rule->child_count) {
        return NULL;
    }

    if (node->type != rule->type) {
        return NULL;
    }
    MathVariableMap *map = calloc(1, sizeof(MathVariableMap));
    if (!populate_variable_map(map, node, rule)) {
        free_variable_map(map);
        return NULL;
    };

    printf("map var count: %ld\n", map->variable_count);

    return map;
}


AstNode *variable_map_get_variable_value(MathVariableMap *map, char *name) {
    int index = math_variable_map_get_name_index(map, name);
    if (index == -1) {
        return NULL;
    }

    AstNode *response = map->_variable_values[index];
    printf("GETTING VALUE %s\n", name);
    printf("value -> ");
    print_ast_as_string(response);

    return response;
}
