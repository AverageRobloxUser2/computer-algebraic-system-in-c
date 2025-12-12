#include "simplifier.h"
#include "hash_table.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "simplifier.h"
#include <string.h>
#include <limits.h>

void compact_add(ExpresionNode *node) {
    if (node->is_leaf) {
        return;
    }

    HashTable hash_table_counts = new_hash_table();
    HashTable hash_table_nodes = new_hash_table();


    bool has_sum = false;
    double sum = 0;

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (child->type == TokenTypeNumber) {
            sum += child->value;
            has_sum = true;
            continue;
        }

        char *child_expression = node_to_string(child);
        int *previous_value = get_hash_table(&hash_table_counts, child_expression);

        if (previous_value == NULL) {
            previous_value = malloc(sizeof(int));
            *previous_value = 0;
            set_hash_table(&hash_table_counts, child_expression, previous_value);
            ExpresionNode *previous_node = get_hash_table(&hash_table_nodes, child_expression);
            free(previous_node);
        }

        // cuz hash table stores pointers increasing this increases
        // the value inside of the hash table
        (*previous_value)++;
        set_hash_table(&hash_table_nodes, child_expression, child);
    }

    // remove all children
    while (node->nodes->length > 0) {
        vector_pop(node->nodes);
    }


    for(int i = 0; i < hash_table_counts.current_filled_count; i++) {
        char *key = hash_table_counts.keys[i];
        int *count = get_hash_table(&hash_table_counts, key);

        ExpresionNode *original_node = get_hash_table(&hash_table_nodes, key);
        if(*count == 1) {
            vector_pushback(node->nodes, original_node);
            continue;
        }

        ExpresionNode *multiply_node = create_new_node(TokenTypeOperator, "*");
        ExpresionNode *other_node = create_new_number_node((double)*count); 

        vector_pushback(multiply_node->nodes, other_node);
        vector_pushback(multiply_node->nodes, original_node);
        vector_pushback(node->nodes, multiply_node);
    }

    if (has_sum) {
        ExpresionNode *other_node = create_new_number_node(sum);
        vector_pushback(node->nodes, other_node);
    }

    if(node->nodes->length == 1) {
        ExpresionNode *child = vector_get(node->nodes, 0);
        vector_free(node->nodes);
        node->is_leaf = child->is_leaf;
        node->nodes = child->nodes;
        node->type = child->type;
        node->name = child->name;
        node->value = child->value;
        free(child);
    }

}
