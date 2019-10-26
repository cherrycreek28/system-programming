#include <errno.h>
#include "hw2_p1.h"

int main() {
    // Sample testing main for part 1, one test per function
    // NOTE: nowhere near exhaustive
    //printf("Computer Science Is Fun. has %d 'e's (should be 3)\n",
            //contains("ComputerScience Is Fun.", 'e'));
  
    //printf("hello"); 
    //printf("%s", prefix("eeeello", "e")); 
    errno = 0;
    
    int test_a = ics53atoi("-53"); 
   printf("%d\n", test_a);

    float test_b = ics53atof("2.4"); 
   printf("%f\n", test_b);

    
    int test_atoi = ics53atoi("-2.4");
    int test_errno = errno;
    printf("ics53atoi(\"-2.4\") is %d with errno %d, (should be 0 with errno %d)\n", test_atoi, test_errno, INTERR);
    errno = 0;
    printf("ics53atof(\"-2.4\") is %f (should be -2.4)\n", ics53atof("-2.4"));
    //char* toSubstring2 = "-1 22.3       +";
    char* toSubstring = malloc(16*sizeof(char));
    char* toSubstringSeek = toSubstring;
#include<string.h> //strcpy (needed so toSubstring may be modified)
    
    int a = 0; 
    
    strcpy(toSubstring, "hello i dont kn"); 




//while ((*toSubstringSeek++ = *toSubstring2++));
//    toSubstring2 -= 16;
    int size;
    
    
    const char** substrings = (const char**) getSubstrings(toSubstring, &size);
  
   	 

    
    
    printSubstrings(substrings, size);
    free(substrings);
    void* value = malloc(sizeof(float) > sizeof(int) ?
            sizeof(float) : sizeof(int));
    free(toSubstring);
    return 0;
}
