// Needed C stdlib headers
// Do not add to this list
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Error Constants

#define INTERR -1
#define FLTERR -2

// Part 1 Functions

int contains(const char* str, const char c);

char* prefix(const char* str, const char* tokens);

int ics53atoi(const char *str);

float ics53atof(const char *str);

char** getSubstrings(char *str, int *size);

void printSubstrings(const char** sstrs, const int size);
