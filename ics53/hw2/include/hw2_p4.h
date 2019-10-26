#ifndef HW3_P3_H
#define HW3_P3_H
#include <math.h>
#include "hw2_p2.h"

#define TYPERR 1
#define ENOVAL 2
#define ENOOP 3

Token* performOp(Token* op, Token* value1, Token* value2);

Token* evaluatePostfix(char* exp);

#endif
