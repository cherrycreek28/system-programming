// Define any helper functions here
//
//
//
//
#include "helpers2.h"
int power(int a)
{
	int base = 1; 
	while (a>=2)
	{
		base*=10; 
		a--; 
	}
	return base; 
}


int intPower(int a, int b)
{
	int base = 1; 
	for (b; b>0; b--)
	{
		base*=a; 		
	}
	return base; 
}






