#include <errno.h>
#include "hw2_p4.h"
#include "stack.h"
#include "hw2_p3.h"
#include "helpers2.h"

Token* performOp(Token* op, Token* value1, Token* value2) {
	
	Token* newT = malloc(sizeof(Token)); 
	
	if (value1->type == FLOAT || value2->type == FLOAT)
	{

		float op1; 
		float op2; 
	

		if (value1->type == FLOAT)
		{
			op1 = *(float*)value1->datavalue;
		}
		else if (value1->type == INTEGER)
		{
			op1 = *(int*)value1->datavalue; 
		}
		if (value2->type == FLOAT)
		{	
			op2 = *(float*)value2->datavalue; 
		}
		else if (value2->type == INTEGER)
		{	
			op2 = *(int*)value2->datavalue; 
		}

		//printf("%f and %f", op1, op2); 

		float* result = malloc(sizeof(float)); 
		newT->type = FLOAT; 
		
		if (*(char*)(op->datavalue) == '+')
		{
			*result = op1 + op2; 		
		}
		if (*(char*)(op->datavalue) == '*')
		{
			*result = op1 * op2; 		
		}
		
		if (*(char*)(op->datavalue) == '-')
		{
			*result = op1 - op2; 		
		}

		if (*(char*)(op->datavalue) == '/')
		{
			if (op2 == 0)
			{
				errno = TYPERR;
				return NULL;
			}

			*result = op1 / op2; 		
		}


		if (*(char*)(op->datavalue) == '^')
		{
			errno = TYPERR;
			return NULL;
		}
		if (*(char*)(op->datavalue) == '%')
		{
			errno = TYPERR;
			return NULL;
		}

		newT->datavalue = result;
	
	}

	else
	{
		int* result = malloc(sizeof(int)); 
		newT->type = INTEGER; 
		if (*(char*)(op->datavalue) == '+')
		{
			*result = *(int*)value1->datavalue + *(int*)value2->datavalue; 		
		}
		if (*(char*)(op->datavalue) == '*')
		{
			*result = *(int*)value1->datavalue * (*(int*)value2->datavalue); 		
		}
		
		if (*(char*)(op->datavalue) == '-')
		{
			*result = *(int*)value1->datavalue - *(int*)value2->datavalue; 		
		}

		if (*(char*)(op->datavalue) == '/')
		{

			if (*(int*)value2->datavalue == 0)
			{
				errno = TYPERR;
				return NULL;	
			}

			*result = *(int*)value1->datavalue / *(int*)value2->datavalue; 		
		}

		if (*(char*)(op->datavalue) == '^')
		{
			*result = intPower(*(int*)value1->datavalue, *(int*)value2->datavalue); 		
		}

		if (*(char*)(op->datavalue) == '%')
		{
			*result = *(int*)value1->datavalue % *(int*)value2->datavalue; 		
		}

		newT->datavalue = result;
	
	}

	free(op);
       	free(value1); 
	free(value2); 	


	return newT; 	

}

Token* evaluatePostfix(char* exp) { 
	
	
	//StackInfo* stack = malloc(sizeof(StackInfo));
	StackInfo stack; 
	//stack->head = NULL;
	stack.head = NULL; 
	//stack->depth = 0; 	
	stack.depth = 0; 

	Token* next; 

	while ((next = tokenizeCalc(&exp)) != NULL)
	{
		if (next->type == INTEGER || next->type == FLOAT)
		{
			push(&stack, next); 
		}
		else if (next->type == SYMBOL)
		{
			if (stack.depth<2)
			{
				errno = ENOVAL; 
				return NULL;
			}
			
			Token* val2 = pop(&stack); 
			Token* val1 = pop(&stack); 
			push(&stack, performOp(next, val1, val2)); 
		}	
	}

	if (stack.depth == 1)
		//return peek(stack);
		return pop(&stack); 

	else
	{
		errno = ENOOP; 
		empty(&stack); 
		return NULL; 
	}	


}
