#include "ast_tree.h"
#include <stdio.h>
#include <stdlib.h>

void ast_print_define_labels(AstNode *node) {
    printf("\tptr_%p[label=\"%s\"];\n", (void*)node, node->name);

    for (size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[node->child_count-i-1];
        ast_print_define_labels(child);
    }
}

void ast_print_define_edges(AstNode *node) {
    for (size_t i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[node->child_count-i-1];
        printf("\tptr_%p -> ptr_%p;\n", (void*)node, (void*)child);

        ast_print_define_edges(child);
    }
}

void print_ast_tree_as_graphviz(AstNode *node) {
    printf("digraph ast_tree {\n");
    ast_print_define_labels(node);
    printf("\n\n");
    ast_print_define_edges(node);
    printf("}\n");
}

void print_ast_as_string(AstNode *node) {
    char *as_string = ast_node_to_string(node);
    printf("%s\n", as_string);
    free(as_string);
}
