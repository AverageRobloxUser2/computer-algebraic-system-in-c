#include "ast_tree.h"
#include "math_equation.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int find_child_index(AstNode *node, AstNode *to_match_node) {
    for(int i = 0; i < node->child_count; i++) {
        AstNode *child = node->children_ptrs[i];
        if (ast_node_is_same_node(child, to_match_node)) {
            return i;
        }
    }

    return -1;
}

void append_child_node(AstNode *parent, AstNode *child) {
    parent->child_count ++;
    parent->children_ptrs = reallocarray(
        parent->children_ptrs,
        parent->child_count,
        sizeof(AstNode*)
    );

    parent->children_ptrs[parent->child_count-1] = child;
}


void remove_and_free_child_at_index(AstNode *parent, size_t index) {
    if (parent->child_count <= index) {
        return;
    }

    AstNode **new_children = calloc(
        sizeof(AstNode*), 
        parent->child_count-1
    );

    size_t j = 0;
    for(size_t i = 0; i < parent->child_count; i++) {
        AstNode *child =  parent->children_ptrs[i];
        if (i == index) {
            free_ast(child);
            continue;
        }
        new_children[j] = child;
        j++;
    }

    free(parent->children_ptrs);
    parent->children_ptrs = new_children;
    parent->child_count--;
}

void remove_and_free_child(AstNode *parent, AstNode *child) {
    int index = find_child_index(parent, child);
    if (index == -1) {
        return;
    }

    remove_and_free_child_at_index(parent, index);
}

void replace_node_with_another(AstNode *node, AstNode *replacment) {
    // consumes the replacment node ptr
    free(node->name);
    free(node->children_ptrs);

    node->type = replacment->type;
    node->children_ptrs = replacment->children_ptrs;
    node->child_count = replacment->child_count;
    node->name = replacment->name;
    node->value = replacment->value;

    free(replacment);
}

AstNode *create_number_node(double number) {
    char number_as_string[32];
    sprintf(number_as_string, "%lf", number);
    AstNode *new_node = create_new_node(MathNumberToken, number_as_string);

    return new_node;
}

bool ast_node_is_same_node(AstNode *node_a, AstNode *node_b) {
    if (node_a == node_b) {
        return true;
    }

    if (node_a == NULL || node_b == NULL) {
        return false;
    }

    if (node_a->type != node_b->type) {
        return false;
    }

    if (node_a->child_count != node_b->child_count) {
        return false;
    }

    if (strcmp(node_a->name, node_b->name)) {
        return false;
    }

    for(size_t i = 0; i < node_a->child_count; i++) {
        AstNode *child_a = node_a->children_ptrs[i];
        AstNode *child_b = node_b->children_ptrs[i];
        if (!ast_node_is_same_node(child_a, child_b)) {
            return false;
        }
    }

    return true;
}
