#ifndef POSTFIX_H
#define POSTFIX_H

#include "infix.h"
#include "math_equation.h"

typedef MathEquation PostfixEquation;

PostfixEquation convert_infix_to_postfix(InfixEquation equation);

#endif
