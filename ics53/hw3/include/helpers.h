#ifndef HELPERS_H
#define HELPERS_H

#include "icsmm.h"
#include <stdio.h>


/* Helper function declarations go here */
int spaceNeeded(int request, int* padding_amount); 
void* safeAllocate(ics_free_header* currentHeader, int padding_amount, int space); 
void* shrinkAllocatedBlock(ics_free_header* currentHeader, int padding_amount, int space); 

//try add more pages
int morePage(ics_free_header* lastFreeBlock); 



















#endif
