#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>

typedef struct {
    size_t length;
    void **_values;
} Vector;

Vector *vector_new();
void *vector_get(Vector* vector, size_t index);
void vector_pushback(Vector* vector, void *value_ptr);
void vector_set(Vector *vector, size_t index, void *value_ptr);
void vector_free(Vector *vector);
void *vector_pop(Vector *vector);
void vector_remove(Vector *vector, size_t index);
#endif
