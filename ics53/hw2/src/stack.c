// Stack implementation via Linked list
#include <stdio.h> 
#include <stdlib.h>
#include <limits.h>
#include "stack.h"


void* newNode(void** data)
{
	StackNode* current = (StackNode*)malloc(sizeof(StackNode));
	current->stackValue = *data; 
	current->next = NULL; 
	return current; 	
}

// Functions to modify
/*StackNode* newNode(int data) 
{ 
	StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode)); 
	stackNode->data = data; 
	stackNode->next = NULL; 
	return stackNode; 
}*/

int isEmpty(StackInfo* data)
{
	return !(data->depth); 
}

/*int isEmpty(StackNode* root) 
{ 
	return !root; 
} */ 

void push(StackInfo* stack, void* value)
{
	StackNode* old = stack->head; 
	stack->head = newNode(&value); 
	//stack->head = newNode(&value);
	//stack->head = newNode(value); 	
	(stack->head)->next = old; 
	stack->depth++; 	
	//printf("%p pushed to stack\n", value); 
}

/*

void push(StackNode** root, int data) 
{ 
	StackNode* stackNode = newNode(data); 
	stackNode->next = *root; 
	*root = stackNode; 
	printf("%d pushed to stack\n", data); 
} */

void* pop(StackInfo* stack)
{
	if (stack->depth>0)
	{
		void* needReturn = stack->head->stackValue; 
		StackNode* needFree = stack->head; 

		stack->head = stack->head->next; 
		stack->depth--; 
		//printf("popped"); 
		free(needFree); 
		return needReturn; 

	}
}
/*int pop(StackNode** root) 
{ 
	if (isEmpty(*root)) 
		return INT_MIN; 
	struct StackNode* temp = *root; 
	*root = (*root)->next; 
	int popped = temp->data; 
	free(temp); 

	return popped; 
} */

/*
int peek(StackNode* root) 
{ 
	if (isEmpty(root)) 
		return INT_MIN; 
	return root->data; 
} */

void* peek(StackInfo* stack)
{
	if (stack->depth > 0)
		return stack->head->stackValue; 
	return NULL; 
}

