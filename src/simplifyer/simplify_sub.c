#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void compact_sub(ExpresionNode *node) {
    Vector *to_remove = vector_new();
    double result = 0;
    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);

        if (child->type != TokenTypeNumber) {
            continue;
        }

        if(i == 0) {
            result += child->value;
        } else {
            result -= child->value;
        }

        int *value = malloc(sizeof(int));
        *value = i;

        // very weird hack we cast to void* so we can then uncast to (int) 
        // because the vector_pushback signature is Vector* and void*
        vector_pushback(to_remove, value);
    }
    remove_nodes(node->nodes, to_remove);
    vector_free(to_remove);
    ExpresionNode *new_node;

    new_node = create_new_number_node(result);
    if (node->nodes->length == 0) {
        vector_free(node->nodes);
        node->nodes = NULL;
        node->is_leaf = true;
        node->type = TokenTypeNumber;
        node->value = result;
        node->name = new_node->name;

        free(new_node);
    } else {
        vector_pushback(node->nodes, new_node);
    }
}
