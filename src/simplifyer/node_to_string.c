#include "simplifier.h"
#include "tokenizer.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <strings.h>

void sort_argument_nodes(Vector *nodes) {
    for(int i = 0; i < nodes->length; i++) {
        char *best_alphabeticaly_string = NULL;
        int best_j = 0;

        for(int j = i+1; j < nodes->length; j++) {
            ExpresionNode *node_2 = vector_get(nodes, j);
            char *node_2_name = node_to_string(node_2);

            if (best_alphabeticaly_string == NULL) {
                best_alphabeticaly_string = node_2_name;
            } else if (strcasecmp(
                            best_alphabeticaly_string,
                            node_2_name
                        ) > 0) {
                free(best_alphabeticaly_string);

                best_alphabeticaly_string = node_2_name;
                best_j = j;
            }
        }

        if (best_alphabeticaly_string == NULL) {
            break;
        }
        ExpresionNode *node_1 = vector_get(nodes, i);
        char *node_1_name = node_to_string(node_1);

        if (strcasecmp(best_alphabeticaly_string, node_1_name) > 0) {
            vector_swap(nodes, i, best_j);
        }

        free(node_1_name);
        free(best_alphabeticaly_string);
    }
}

char *node_to_string(ExpresionNode *node) {
    char *result = calloc(100, sizeof(char));
    switch (node->type) {
        case TokenTypeUnaryOperator:
            sprintf(result, "unary_%s(", node->name);
            break;

        case TokenTypeOperator:
            sprintf(result, "op_%s(", node->name);
            break;

        case TokenTypeFunction:
            sprintf(result, "func_%s(", node->name);
            break;

        case TokenTypeVariable:
            sprintf(result, "var(\"%s\")", node->name);
            break;

        default:
            sprintf(result, "%s", node->name);
            break;
    }
    if (node->is_leaf) {
        return result;
    }

    sort_argument_nodes(node->nodes);

    for(int i = 0; i < node->nodes->length; i++) {
        ExpresionNode *child = vector_get(node->nodes, i);
        int old_length = strlen(result);

        char *child_string = node_to_string(child);
        int child_length = strlen(child_string);

        result = reallocarray(
            result,
            old_length + child_length + 2,
            sizeof(char)
        );

        strcpy(result + old_length, child_string);
        free(child_string);
        result[child_length + old_length] = ',';
        result[child_length + old_length + 1] = '\0';
    }

    int old_length = strlen(result);

    // result = reallocarray(result, old_length + 2, sizeof(char));
    result[old_length - 1] = ')';
    result[old_length] = '\0';

    return result;
}
