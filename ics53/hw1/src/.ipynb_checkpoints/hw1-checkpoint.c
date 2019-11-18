// Qingchuan Yang
// qingchy

#include "hw1.h"
// You may define any helper functions you want. Place them in helpers.c/.h

// Main program
int main(int argc, char *argv[]) 
{
    	char c;

	if (argc == 1)
	{
		fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
		return 1; 
	}


	if (strcmp(argv[1],"-T") == 0)
	{

		if (argc != 2)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1; 
		}

		

		while ((c = getchar()) !=EOF)
		{
			if (c == ',')
			{
				c = '\t'; 
			}	
			printf("%c", c); 
		}

		return 0; 
	
	}
	else if (strcmp(argv[1],"-V") == 0)
	{
		return validate(); 
	}
	else if (strcmp(argv[1],"-E") == 0)
	{
		/*if (argc != 3)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1; 
		}
		
		
		
		
		int eliminateCol = strtol(argv[2],NULL,10); 
		int n = 1; 
		int col = 1; 
		
		if (eliminateCol<=0)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1; 
		}
		while ((c = getchar()) != EOF)
		{

			if (c == ',')
				n++; 
			if ((n!=eliminateCol && !(eliminateCol == 1 && (c == ',') && n == 2)) || c == '\n')
				printf("%c", c); 
			if (c == '\n')
			{
				if (eliminateCol>n)
				{
					printf("%d%d", eliminateCol, n); 
					fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
					return 1; 
				}
				n = 1; 
			}
		}*/
		if (argc != 3)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;	
		}
		
		int col = strtol(argv[2], NULL, 10); 
		if (col<=0)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;
		}
		char buf[200]; 

		int n = 0; 

		scanf("%[^\n]s", buf); 

		char d[] = ","; 
		char* ptr = strtok(buf, d); 
		
		while (ptr != NULL)
		{
			n++; 
			
			ptr = strtok(NULL, d); 
		}
			
		if (col>n)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;
		}
		
		else
		{
			char d[] = "\0"; 
			rewind(ptr); 
			ptr = strtok(buf, d); 
			
			while (ptr != NULL)
			{
				printf("%s", ptr); 
				n++; 

				if (n != col)
				{	
					printf("%s", ptr); 
				}
				ptr = strtok(NULL, d); 
			}
	
		}
		
		n = 0; 
		char c = getchar(); 
		printf("%c", c);

		while (scanf("%[^\n]s", buf) != EOF)
		{
					
			char d[] = ","; 
			char* ptr = strtok(buf, d); 
			
			while (ptr != NULL)
			{
				n++; 

				if (n != col)
				{	
					printf("%s", ptr); 
				}
				ptr = strtok(NULL, d); 
			}
				
			//getchar(); 
			c = getchar(); 
			printf("%c", c);
			n = 0; 
		}
	
		return 0; 
		//return 0; 
	
	}
	else if (strcmp(argv[1], "-M") == 0)
	{
		if (argc != 4)
		{

			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;
		}
		int col = strtol(argv[3], NULL, 10); 
       		char* match = argv[2]; 	       
	
		if (argc != 4)
		{

			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;
		}
		if (col<=0)
		{
			fprintf(stderr, "usage: 53csv -T | -V\n\t53csv -E COL_NUM\n\t53csv -M STRING_TO_MATCH COL_NUM\n"); 
			return 1;
		}
		char buf[200]; 

		int count = 0; 
		int n = 0; 

		while (scanf("%[^\n]s", buf) != EOF)
		{
			//printf("%s", buf); 
					
			char d[] = ","; 
			char* ptr = strtok(buf, d); 
			
			while (ptr != NULL)
			{
				n++; 

				if (n == col && strstr(ptr, match) != NULL)
				{	

					count+=1; 
				}


				ptr = strtok(NULL, d); 
				//printf()
			}
					
			
			getchar(); 
			n = 0; 
		}
	
		printf("%d\n", count); 
		return 0; 
	}


	return 2; 

}
