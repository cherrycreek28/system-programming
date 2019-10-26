#ifndef HW3_P1_H
#define HW3_P1_H


#include <stdio.h>


typedef struct {
    enum datatype {SYMBOL, INTEGER, FLOAT} type;
    void* datavalue;
} Token;

Token* tokenizeCalc(char** str);

#endif
