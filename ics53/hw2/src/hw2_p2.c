#include "hw2_p1.h"
#include "hw2_p2.h"
#include "helpers2.h"

Token* tokenizeCalc(char** str) {
	
	char* old = *str; 

	//*str = prefix(*str, " \t\0"); 
	if (**str != '\0')
	{

		int i = 0; 

		if (**str == 42 || **str == 43 || **str == 47 || **str == 94 || **str == 37)	
		{
			char* cCopy = malloc(sizeof(char)); 
			*cCopy = **str; 
			Token* newT = malloc(sizeof(Token)); 
			newT->type = SYMBOL;
			newT->datavalue = cCopy;	
			*str += 1; 
			*str = prefix(*str, " \t\n");
			return newT; 	
		}

		if (**str == 45)
		{
			if (*(*str+1) > '9' || *(*str+1) < '0')
			{
				char* cCopy = malloc(sizeof(char)); 
				*cCopy = **str; 
				Token* newT = malloc(sizeof(Token)); 
				newT->type = SYMBOL;
				newT->datavalue = cCopy;	
				*str += 1; 
				*str = prefix(*str, " \t\n");
				return newT; 	
			}

			i++; 
		}

		if (*(*str+i)>='0' && *(*str+i)<='9')
		{
					
			while (*(*str+i)>='0' && *(*str+i)<='9') 
			{
				i++; 
			}
			
			if (*(*str+i) == '.')
			{

				i++; 

				while (*(*str+i)>='0' && *(*str+i)<='9') 
				{
					i++; 
				}

				if (*(*str+i) == 42 || *(*str+i) == 43 || *(*str+i) == 47 || *(*str+i) == 94 || *(*str+i) == 37 || *(*str+i) == 45)	
				{
					*str = old; 
					errno = EINVAL;
					return NULL; 
				}
				
				char* strCopy = malloc(i); 

				int n = 0; 
				
				for (n; n<i; n++)
				{
					*(strCopy+n) = *(*str+n); 
				}

				*(strCopy+i) = '\0'; 


				float* fCopy = malloc(sizeof(float)); 
				*fCopy = ics53atof(strCopy); 
				

				Token* newT = malloc(sizeof(Token)); 
				newT->type = FLOAT;
				newT->datavalue = fCopy; 
				
				free(strCopy);
				*str = *str+i; 
				*str = prefix(*str, " \t\n"); 
				return newT; 

			}
			
			else if (*(*str+i) == 42 || *(*str+i) == 43 || *(*str+i) == 47 || *(*str+i) == 94 || *(*str+i) == 37 || *(*str+i) == 45)	
			{
				*str = old; 
				errno = EINVAL;
				return NULL; 
			}
			
			else	
			{

				char* strCopy = malloc(i); 

				int n = 0; 
				for (n; n<i; n++)
				{
					*(strCopy+n) = *(*str+n); 
				}

				*(strCopy+i) = '\0'; 


				int* iCopy = malloc(sizeof(int)); 
				*iCopy = ics53atoi(strCopy); 
				

				Token* newT = malloc(sizeof(Token)); 
				newT->type = INTEGER;
				newT->datavalue = iCopy; 

				free(strCopy);
			       	*str = *str+i; 	
				*str = prefix(*str, " \t\n"); 
				return newT;
			}



		}

		else
		{
			*str = old; 
			errno = EINVAL;
			return NULL; 
		}


		//*str = *str+1; 
			
	}
	else if (**str == '\0')
	{
		return NULL; 	
	}



}
