#include "helpers2.h"
#include "hw2_p1.h"

int contains(const char* str, const char c) {
	if (str == NULL || *str == '\0')
		return -1;
	else
	{
		unsigned int count = 0; 
		unsigned int i = 0; 
		while (*(str+i) != '\0')
		{
			if (*(str+i) == c)
			{
				count++; 
			}
			i++; 
		}
		return count;
	}
}

char* prefix(const char* str, const char* tokens) {
	if (str == NULL || tokens == NULL)
	{
		return NULL; 
	}

	unsigned int i = 0; 

	while (*(str+i) != '\0')
	{
		int flag = 0;

		unsigned int j = 0; 
		while (*(tokens+j) != '\0')
		{
			if (*(str+i) == *(tokens+j))
			{
				flag = 1; 
				i++; 
			       	break;
			}
			j++; 	       			       
		}
		if (flag == 0)
			return (char* )str+i; 
	}	
	return (char* )str+i; 


}

int ics53atoi(const char *str) {
	if (str == NULL)
	{
		errno = INTERR;
		return 0; 
	}
	
	int i = 0;
	int n = 0;
	int nega = 1; 	

	while (*(str+i) != '\0')
	{
		//printf("hello"); 


		if (*(str+i) == 45 && i == 0 && *(str+i+1) != '\0')
		{
			nega=-1;
		}

		else if (*(str+i)<'0' || *(str+i)>'9')
		{
			errno = INTERR; 
			return 0; 
		}
		
		i++; 
		n++; 
	}

	int sum = 0; 
	i = 0; 
	while (*(str+i) != '\0')
	{
		if (*(str+i)>='0' && *(str+i)<='9')
			sum+=(int)(*(str+i) - '0')*(power(n)); 
		n--; 
		i++; 
	}

	//printf("%d", nega*sum); 
	return nega*sum; 
}

float ics53atof(const char *str) {

	if (str == NULL)
	{
		errno = FLTERR;
		return 0; 
	}
	
	int i = 0;
	int n = 0;
	int nega = 1;
	int fp = 0; 	

	while (*(str+i) != '\0')
	{
		if (*(str+i) == 45 && i == 0 && *(str+i+1) != '\0')
		{
			nega=-1;
		}

		else if (*(str+i) == '.')
		{
			fp = n; 
		}

		else if (*(str+i)<'0' || *(str+i)>'9')
		{
			errno = FLTERR; 
			return 0; 
		}
		
		else
		{
			n++; 
		}
			
		i++; 
	}
	int nCopy = n; 
	int sum = 0; 
	i = 0; 
	while (*(str+i) != '\0')
	{
		if (*(str+i)>='0' && *(str+i)<='9')
		{
			sum+=(int)(*(str+i) - '0')*(power(n)); 
			n--; 
		}
		i++; 
	}
	
	//printf("%d, %d, %d, %d\n", nCopy, fp, power(nCopy-fp+1), sum); 
	

	float result = (float)nega*sum/(float)(power(nCopy-fp+1)); 
	return result; 


}


char** getSubstrings(char *str, int *size) {
	//char** ptr = (char** )malloc(sizeof(char*)); 
	char** ptr; 
	int i = 0;
	int p = 1; 
       	int flag = 1; 	
	while (*(str+i) != '\0')
	{
		if (*(str+i) != ' ')
		{
			if (flag == 1)
			{
				ptr = realloc(ptr, sizeof(char*) * p); 	
				
				*(ptr+p-1) = str+i;
			       	p++; 	
				flag = 0; 	
			}
		}
		else if (*(str+i) == ' ')
			{
				*(str+i) = '\0'; 
				flag = 1; 
			}		
		i++; 
	}
	*size = p-1; 
	return ptr; 
}

void printSubstrings(const char** sstrs, const int size) {
	if (size!=0 && sstrs != NULL)
	{
		printf("{");
		int n = 1; 	
		while (n<=size)
		{
			if (n!=1)
				printf(","); 

			printf("%s", *(sstrs+n-1)); 
			n++; 
		}
		printf("}\n"); 
	}
}
