#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    char *input;
    char *output;
} TestCase;

void print_as_passed(TestCase test_case) {
    printf(ANSI_COLOR_GREEN "PASSED: %s -> %s" ANSI_COLOR_RESET "\n", 
            test_case.input,
            test_case.output);
}

void print_as_failed(TestCase test_case, char *output) {
    printf(ANSI_COLOR_RED "FAILED: %s -> \"%s\"\n", test_case.input, test_case.output);
    printf("GOT: \"%s\"\n" ANSI_COLOR_RESET, output);
}

#endif
