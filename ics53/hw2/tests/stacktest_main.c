#include <stdio.h> 
#include <stdlib.h> 
#include <stack.h>

int main() 
{ 
    StackInfo* stack = malloc(sizeof(StackInfo));
    stack->depth = 0;
    stack->head = NULL;  
    
    push(stack, (void*) 10);
    push(stack, (void*) 20);
    push(stack, (void*) 30);

    //printf("%d popped from stack\n", pop(stack));

	printf("%d", stack->depth); 


    pop(stack);	
  	pop(stack); 
	pop(stack); 
	pop(stack);

    //printf("Top element is %p\n", peek(stack));

    return 0; 
} 
