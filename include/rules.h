#ifndef SIMPLIFY_RULES_H
#define SIMPLIFY_RULES_H
typedef struct {
    char *left;
    char *right;
} MathSimplificationRule;

MathSimplificationRule rules[] = {
    {"--a", "a"}
};
#endif
