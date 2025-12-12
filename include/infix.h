#ifndef INFIX_H
#define INFIX_H
#include "lexer.h"
#include "math_equation.h"

typedef MathEquation InfixEquation;

InfixEquation convert_lexed_to_infix(LexerResult lexed);

#endif
