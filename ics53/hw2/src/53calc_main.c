#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "hw2_p4.h"

#define TYPERR_MESSAGE "There was a mismatch of types in the calculation\n"
#define ENOVAL_MESSAGE "There was no value for an operation in the calculation\n"
#define ENOOP_MESSAGE "There was not enough operators in the calculation\n"
#define GENERAL_ERR_MESSAGE "Some unidentifiable error occurred in the calculation\n"

int main() {
    int count = 0;  // for return value to be the number of expressions with errors
    size_t n = 0;
    char* line = NULL;
    while (getline(&line, &n, stdin) != -1) {
	  Token* value = evaluatePostfix(line);
	  
	  if(value == NULL) {
              switch (errno) {
                  case (TYPERR):
                      printf(TYPERR_MESSAGE);
                      break;
                  case (ENOVAL):
                      printf(ENOVAL_MESSAGE);
                      break;
                  case (ENOOP):
                      printf(ENOOP_MESSAGE);
                      break;
                  default:
                      printf(GENERAL_ERR_MESSAGE);
                      break;
              }
              errno = 0;
              ++count;
	  }
	  else
	  {
	     if (value->type == SYMBOL) {
                 return -1;
             } else if (value->type == INTEGER) {
                 printf("%d\n", *((int*) value->datavalue));
             } else if (value->type == FLOAT) {
                 printf("%f\n", *((float*) value->datavalue));
             }
             else {
                 return -1;
             }
        }
    }
    if (line) {
        free(line);
    }
    return count;
}
