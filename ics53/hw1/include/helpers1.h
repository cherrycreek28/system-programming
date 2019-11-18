// Declare all helper functions for hw1 in this file


#include <stdio.h>

int validate()
{
	char c;
	int row = 0; 
	int col = 0;
	int n = 0; 

	while ((c = getchar()) != EOF)
	{
		if (c == ',')
		{
			n++;  
		}
		if (c == '\n')
		{
			row++; 
			if (col != n+1 && row!=1)
			{
				fprintf(stderr, "%d\n", row); 
				return 2; 
			}
			col = n+1; 
			n = 0; 
		}
	}
	printf("%d %d\n", row, col); 
	return 0;
}


int validateHelper(int arr[])
{
	char c;
	int row = 0; 
	int col = 0;
	int n = 0; 

	while ((c = getchar()) != EOF)
	{
		if (c == ',')
		{
			n++;  
		}
		if (c == '\n')
		{
			row++; 
			if (col != n+1 && row!=1)
			{
				return 1; 
			}
			col = n+1; 
			n = 0; 
		}
	}
	//printf("%d %d\n", row, col); 
	arr[0] = row; 
	arr[1] = col; 
	
	return 0; 
}	
