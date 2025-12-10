#include "vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

Vector *vector_new() {
    Vector *vector = (Vector*) malloc(sizeof(Vector));

    if (vector == NULL) {
        return NULL;
    }
    vector->_values = malloc(0);
    vector->length = 0;

    return vector;
}


void *vector_get(Vector* vector, size_t index) {

    if (vector->length <= index) {
        return NULL;
    }

    return vector->_values[index];
};

void vector_pushback(Vector* vector, void *value_ptr) {
    vector->length ++;

    if (vector->_values == NULL) {
        vector->_values = malloc(sizeof(void*) * vector->length);
    } else {
        vector->_values = reallocarray(
            vector->_values,
            vector->length,
            sizeof(void*)
        );
    }

    vector->_values[vector->length - 1] = value_ptr;
};
void vector_set(Vector *vector, size_t index, void *value_ptr) {

    if (vector->length <= index) {
        return;
    }

    vector->_values[index] = value_ptr;
};

void *vector_remove(Vector *vector, size_t index) {
    void **new_values = calloc(vector->length-1, sizeof(void*));
    void *result;
    int found_count = 0;
    for(int i = 0; i < vector->length; i++) {
        if(i == index) {
            result = vector_get(vector, i);
            continue;
        }

        new_values[found_count] = vector_get(vector, i);
        found_count++;
    }

    free(vector->_values);
    vector->_values = new_values;
    vector->length --;

    return result;
}

void vector_free(Vector *vector) {
    if (vector->_values != NULL) {
        free(vector->_values);
    }
    free(vector);
}

void *vector_pop(Vector *vector) {
    void *last_value = vector_get(vector, vector->length - 1);
    vector->length--;

    if (vector->length > 0) {
        vector->_values = reallocarray(
            vector->_values,
            vector->length,
            sizeof(void*)
        );
    } else {
        free(vector->_values);
        vector->_values = NULL;
    }

    return last_value;
}
