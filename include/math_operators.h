#ifndef MATH_OPERATORS_H
#define MATH_OPERATORS_H
#include "vector.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool add_operator(Vector *stack) {
    if (stack->length < 2) {
        return false;
    }

    double *b = vector_pop(stack);
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = *a + *b;

    free(b);
    free(a);

    vector_pushback(stack, c);

    return true;
}

bool subtract_operator(Vector *stack) {
    if (stack->length < 2) {
        return false;
    }

    double *b = vector_pop(stack);
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = *a - *b;

    free(b);
    free(a);


    vector_pushback(stack, c);

    return true;
}

bool multiply_operator(Vector *stack) {
    if (stack->length < 2) {
        return false;
    }

    double *b = vector_pop(stack);
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = *a * *b;

    free(b);
    free(a);


    vector_pushback(stack, c);

    return true;
}

bool divide_operator(Vector *stack) {
    if (stack->length < 2) {
        return false;
    }

    double *b = vector_pop(stack);
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    if (*b == 0) {
        *c = 0;
    } else {
        *c = *a / *b;
    }


    free(b);
    free(a);


    vector_pushback(stack, c);

    return true;
}

bool power_operator(Vector *stack) {
    if (stack->length < 2) {
        return false;
    }

    double *b = vector_pop(stack);
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = pow(*a, *b);

    free(b);
    free(a);

    vector_pushback(stack, c);

    return true;
}

bool factorial_operator(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = 1;

    for(int i = 2; i <= *a; i++) {
        *c *= i;
    }

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool negate_operator(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = -*a;
    free(a);

    vector_pushback(stack, c);
    
    return true;
}

bool positive_operator(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);

    double *c = malloc(sizeof(double));

    *c = *a;
    free(a);

    vector_pushback(stack, c);

    return true;
}

#endif
