// Your helper functions need to be here.

#include "helpers.h"

int timeComparator(void* val1, void* val2)
{
    return ((bgentry_t*)val1)->seconds >= ((bgentry_t*)val2)->seconds; 
}

extern int childDieFlag; 
void sigchldHandler(int sig)
{
    //int oldErrno = errno; 
    childDieFlag = 1; 
    //exit(0); 
    //errno = oldErrno; 
}

/* indexed by 0 */
void* removeByPid(List_t* list, int pid) {
    node_t** head = &(list->head);
    void* retval = NULL;
    node_t* current = *head;
    node_t* prev = NULL;
    //int i = 0;

    if (current == NULL)
        return NULL;

    while (((bgentry_t*)(current->value))->pid != pid) {
        prev = current;
        current = current->next;
    }

    if (current == NULL)
        return NULL; 

    if (prev != NULL)
        prev->next = current->next;
    
    retval = current->value;
    free(current);

    list->length--;
    return retval;
}
/*
extern int exec_result; 
extern int exit_status; 
void pipeRedirection(job_info* job)
{
    int n = 1; 
    proc_info* temp = job->procs; 
    int pipeTable[100];  

    int i = 1; 
    for (; i<=job->nproc - 1; i++)
    {
        pipe(pipeTable+(i-1)*2); 
    }

    List_t pidList; 
    pidList.length = 0; 
    pidList.comparator = NULL;
    
    for (; n<=job->nproc; n++)
    {
        int* pid = malloc(sizeof(int)); 
        *pid = fork(); 
        if (*pid == 0)
        {
            //printf("%d", n); 

            if (n == 1)
            {
                dup2(pipeTable[1], 1); 
                close(pipeTable[0]); 	
                close(pipeTable[1]); 
            }

            else if (n < job->nproc && n > 1)
            {
                dup2(pipeTable[2*n-4], 0);
                dup2(pipeTable[2*n-1], 1); 
                close(pipeTable[2*n-4]); 
                close(pipeTable[2*n-1]); 
            }

            else if (n == job->nproc)
            {
                dup2(pipeTable[2*n-4], 0); 
                close(pipeTable[2*n-4]); 
            }

            exec_result = execvp(temp->cmd, temp->argv);

        }
        else
        {
            if (n != job->nproc)
                close(pipeTable[2*n-1]); 
            insertFront(&pidList, pid);
            temp = temp->next_proc; 
        }

    }
    while (pidList.head != NULL)
    {
        waitpid(*(int*)removeFront(&pidList), &exit_status, 0);
    }
	//continue; 
}
*/