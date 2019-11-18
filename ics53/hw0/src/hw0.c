// Your name
// Netid
#include <string.h>


#include "hw0.h"

char h = 'H', e = 'e', l = 'l', term = '\0'; 
char* str = &h; 



int main (int argc, char *argv[])
{
	
	//char str[3] = "XYZ"; 
	
	/*char str[] = {'x', 'y', 'z', 'w'}; 
	
	printf("%d\n", strlen(str)); 
	printf("%d\n", sizeof(str)); 

	printf("%d", (str[4] == '\0')); 
*/

	/*for (int i = 0; i<4; i++)
	{
		printf("%c", *(str+i)); 

	}*/

	printf("%d\n", argc); 






	//Comment/Delete this print statement and insert your hw0 code here 
	//printf("Hello ICS53 student!\n"); 
	

 	int i; 

	for (i = argc-1; i>=0; i--)
	{
		printArg(argv[i], i); 
	}





	return 0;
}









//Function to print out a single arugment to the screen
void printArg(char * arg_str, int pos){

	//Insert your code here
	
	printf("Argument[%d]: %s\n", pos, arg_str); 






	return;
}
