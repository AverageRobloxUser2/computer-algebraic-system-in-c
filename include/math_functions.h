#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H
#include "vector.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool sin_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = sin(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool cos_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = cos(*a);

    free(a);

    vector_pushback(stack, c);
    return true;
}

bool tan_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }
    
    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = tan(*a);

    free(a);

    vector_pushback(stack, c);
    return true;
}

bool asin_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = asin(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool acos_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = acos(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool atan_function(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = atan(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool sqrt_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = sqrt(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool abs_function(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    if (*a < 0) {
        *c = -*a;
    } else {
        *c = *a;
    }
 
    free(a);

    vector_pushback(stack, c);

    return true;
}

bool exp_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = exp(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool ln_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = log(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}


bool log_function(Vector *stack) {

    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);
    double *c = malloc(sizeof(double));

    *c = log10(*a);

    free(a);

    vector_pushback(stack, c);

    return true;
}

bool rad_function(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }

    double *a = vector_pop(stack);

    double *c = malloc(sizeof(double));

    *c = *a * M_PI / 180;

    free(a);

    vector_pushback(stack, c);
    return true;
}

bool deg_function(Vector *stack) {
    if (stack->length < 1) {
        return false;
    }
    double *a = vector_pop(stack);

    double *c = malloc(sizeof(double));

    *c = *a / M_PI * 180;

    free(a);

    vector_pushback(stack, c);
    return true;
}

#endif
