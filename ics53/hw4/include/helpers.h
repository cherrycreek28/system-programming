// A header file for helpers.c
// Declare any additional functions in this file
#include "icssh.h"
#include "linkedList.h"

int timeComparator(void* val1, void* val2); 
void sigchldHandler(int sig); 
void* removeByPid(List_t* list, int pid); 

//struct job_info job; 
//void pipeRedirection(job); 

