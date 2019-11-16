#include "console.h"
#include <stdlib.h>

static inline void lcr3(unsigned int val)
{   
  asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline void halt(void)
{
    asm volatile("hlt" : : );
}

int main(void)
{
    int i; 
    int sum = 0;

    // Initialize the console
    uartinit(); 

    printk("Hello from C\n");

    
    // Create your page table here

    //unsigned int counter = 0; 

/*
    void** ptd_address[2];
    void* pageTable1 = malloc(4096); 
    void* pageTable2 = malloc(4096); 

    ptd_address[0] = pageTable1; 
    ptd_address[1] = pageTable2; 

    //counter = 0; 
    

*/
    //unsigned int* ptd_address[1024]; 
    
    //unsigned int pageTable1[1024]; 
    //unsigned int pageTable2[1024]; 

    int mark = 0b00000011; 
    int m,n; 

    static unsigned int ptd_address[1024]  __attribute__ ((aligned (4096)));;
    static unsigned int pageTable1[1024]  __attribute__ ((aligned (4096)));;
    static unsigned int pageTable2[1024]  __attribute__ ((aligned (4096)));;

    ptd_address[0] = (unsigned int)pageTable1|mark; 
    ptd_address[1] = (unsigned int)pageTable2|mark; 



    for (m = 0; m<1024; m++)
    {
        pageTable1[m] = (0x1000*m)|mark; 
    }

    for (n = 1024; n<2048; n++)
    {
        pageTable2[n - 1024] = (0x1000*n)|mark; 
    }
    
    lcr3((unsigned int)ptd_address);

    for (i = 0; i < 64; i++) {
        int *p = (int *)(i * 4096 * 32);
        sum += *p; 
                
        printk("page\n"); 
    }
    halt(); 
    return sum; 
}


