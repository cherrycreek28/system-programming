#include "helpers.h"

/* Helper function definitions go here */

#define HEADER_SIZE  8
#define FOOTER_SIZE  8 
#define DOUBLE_POINTER_SIZE  16 
#define PROLOGUE_SIZE 8
#define EPILOGUE_SIZE 8



int spaceNeeded(int request, int* padding_amount)
{
    int pL = 8; 
    int eL = 8; 
    int middle; 
    if (request%16>0)
    {
        middle = (request/16+1)*16; 
    }
    else
    {
        middle = request; 
    }
    *padding_amount = middle - request; 
    return pL+eL+middle; 

}


//int space = spaceNeeded(size, &padding_amount); 

//assuming we have enough space; 




void* shrinkAllocatedBlock(ics_free_header* currentHeader, int padding_amount, int space)
{
    int mask = 0xfffc; 
    currentHeader->header.block_size&=mask;
    int sizeofLeft = currentHeader->header.block_size - space; 
    
    //when there is no space for another allocation; 
    if (sizeofLeft < HEADER_SIZE+FOOTER_SIZE+DOUBLE_POINTER_SIZE)
    {
        currentHeader->header.block_size&=mask;
        void* currentFooterAddress = (void*)currentHeader+currentHeader->header.block_size - FOOTER_SIZE;
        
        ics_footer* currentFooter = (ics_footer* )(currentFooterAddress); 
        
        currentFooter->block_size&=mask; 

        currentHeader->header.block_size++;
        currentFooter->block_size++; 

        currentFooter->padding_amount = padding_amount;
        
        if (padding_amount != 0)
        {
            currentHeader->header.block_size+=2;
            currentFooter->block_size+=2;  
        }

        return NULL; 
    }
    //so the structure is currentHeader|currentFooter|nextHeader|nextFooter
    else
    {
        void* nextFooterAddress = (void*)currentHeader+currentHeader->header.block_size - 8;
        ics_footer* nextFooter = (ics_footer*)nextFooterAddress; 
        
        currentHeader->header.block_size = space; 
        void* currentFooterAddress = (void*)currentHeader+currentHeader->header.block_size - 8;

        ics_footer* currentFooter = (ics_footer*)(currentFooterAddress); 
        //currentFooter->block_size&=mask; 

        currentFooter->block_size = space; 
        currentHeader->header.block_size++; 
        currentFooter->block_size++; 
        currentFooter->unused = 0x333333333333; 
        currentFooter->padding_amount = padding_amount; 
        
        if (padding_amount != 0)
        {
            currentHeader->header.block_size+=2; 
            currentFooter->block_size+=2; 
        }

        void* nextHeaderAddress = currentFooterAddress+8; 
        ics_free_header* nextHeader = (ics_free_header* )(nextHeaderAddress); 
        nextHeader->header.block_size = sizeofLeft; 
        nextHeader->header.block_size++; 
        nextHeader->header.unused = 0x1999999999999; 
        
        //here is some different: we want to set the next block still be allocated
        nextHeader->header.block_size++;
        
        nextFooter->block_size = sizeofLeft; 
        nextFooter->block_size++; 
        nextFooter->padding_amount = 0; 
        //nextFooter->unused = 0x333333333333; 

        return (void* )nextHeader+HEADER_SIZE; 
    }


}






void* safeAllocate(ics_free_header* currentHeader, int padding_amount, int space)
{
    //header.block_size&=
    int sizeofLeft = currentHeader->header.block_size - space; 
    //when there is no space for another allocation; 
    if (sizeofLeft < HEADER_SIZE+FOOTER_SIZE+DOUBLE_POINTER_SIZE)
    {
        void* currentFooterAddress = (void*)currentHeader+currentHeader->header.block_size - FOOTER_SIZE;
        
        ics_footer* currentFooter = (ics_footer* )(currentFooterAddress); 
        currentHeader->header.block_size++;
        currentFooter->block_size++; 
        currentFooter->padding_amount = padding_amount;
        
        if (padding_amount != 0)
        {
            currentHeader->header.block_size+=2;
            currentFooter->block_size+=2;  
        }

        if (currentHeader->prev != NULL)
            currentHeader->prev->next = currentHeader->next; 

        else if (currentHeader->prev == NULL)
            freelist_head = currentHeader->next; 

        if (currentHeader->next != NULL)
            currentHeader->next->prev = currentHeader->prev; 
    }
    //so the structure is currentHeader|currentFooter|nextHeader|nextFooter
    else
    {
        void* nextFooterAddress = (void*)currentHeader+currentHeader->header.block_size - 8;
        ics_footer* nextFooter = (ics_footer*)nextFooterAddress; 
        
        currentHeader->header.block_size = space; 
        void* currentFooterAddress = (void*)currentHeader+currentHeader->header.block_size - 8;
        currentHeader->header.block_size++; 
        
        //printf("%p", currentFooterAddress); 
        ics_footer* currentFooter = (ics_footer*)(currentFooterAddress); 
        currentFooter->block_size = space; 
        currentFooter->block_size++; 
        currentFooter->unused = 0x333333333333; 
        currentFooter->padding_amount = padding_amount; 
        
        if (padding_amount != 0)
        {
            currentHeader->header.block_size+=2; 
            currentFooter->block_size+=2; 
        }

        void* nextHeaderAddress = currentFooterAddress+8; 
        ics_free_header* nextHeader = (ics_free_header* )(nextHeaderAddress); 
        nextHeader->header.block_size = sizeofLeft; 
        nextHeader->header.unused = 0x1999999999999; 

        nextFooter->block_size = sizeofLeft; 
        nextFooter->padding_amount = 0; 
        //nextFooter->unused = 0x333333333333; 

        //connect the nextHeader
        if (currentHeader->prev != NULL)
            currentHeader->prev->next = nextHeader; 

        else if (currentHeader->prev == NULL)
            freelist_head = nextHeader; 

        if (currentHeader->next != NULL)
            currentHeader->next->prev = nextHeader; 

        nextHeader->next = currentHeader->next; 
        nextHeader->prev = currentHeader->prev; 

    }

    return (void*)currentHeader+8; 

}

//last block
int morePage(ics_free_header* lastFreeBlock)
{
    // we have to call sbrk(); 
	void* prevEpilogueAddress = ics_inc_brk() - EPILOGUE_SIZE; 
	ics_header* eL = (ics_header*)(ics_get_brk() - EPILOGUE_SIZE); 
	eL->block_size = 0; 

    int newLastFreeBlockSize; 

	//to check if there are other free blocks in the mem. 
	if (lastFreeBlock != NULL)
	{
		//if the last free block is at the end of heap; 
		if ((void*)lastFreeBlock+lastFreeBlock->header.block_size == prevEpilogueAddress)
		{
			lastFreeBlock->header.block_size += ((void*)eL - prevEpilogueAddress); 
			void* lastFooterAddress = (void*)eL - 8; 
			ics_footer* lastFooter = (ics_footer*)lastFooterAddress; 
			lastFooter->block_size = lastFreeBlock->header.block_size; 
			lastFooter->padding_amount = 0; 
			lastFooter->unused = 0x333333333333; 
            newLastFreeBlockSize = lastFreeBlock->header.block_size; 
		}
		//if the last free block is not at the end of heap
		else
		{
			void* currentHeaderAddress = prevEpilogueAddress + 8; 
			ics_free_header* currentHeader = (ics_free_header*)(currentHeaderAddress);
			currentHeader->header.block_size = ics_get_brk() - EPILOGUE_SIZE - prevEpilogueAddress; 
			currentHeader->header.unused = 0x1999999999999; 

			currentHeader->next = NULL; 
			currentHeader->prev = lastFreeBlock;
			
			void* currentFooterAddress = ics_get_brk() - EPILOGUE_SIZE - FOOTER_SIZE;  
			ics_footer* currentFooter = (ics_footer*)(currentFooterAddress);
			currentFooter->block_size = ics_get_brk() - EPILOGUE_SIZE - prevEpilogueAddress;
			currentFooter->padding_amount = 0; 
			currentFooter->unused = 0x333333333333; 
            newLastFreeBlockSize = currentHeader->header.block_size; 
            
            //just for a easy return; 
            lastFreeBlock = currentHeader; 
		}
    }

    else
	{
		void* currentHeaderAddress = prevEpilogueAddress + 8; 
		ics_free_header* currentHeader = (ics_free_header*)(currentHeaderAddress);
		currentHeader->header.block_size = ics_get_brk() - EPILOGUE_SIZE - prevEpilogueAddress; 
		currentHeader->header.unused = 0x1999999999999; 

		currentHeader->next = NULL; 
		currentHeader->prev = lastFreeBlock;
		
		void* currentFooterAddress = ics_get_brk() - EPILOGUE_SIZE - FOOTER_SIZE;  
		ics_footer* currentFooter = (ics_footer*)(currentFooterAddress);
		currentFooter->block_size = ics_get_brk() - EPILOGUE_SIZE - prevEpilogueAddress;
		currentFooter->padding_amount = 0; 
		currentFooter->unused = 0x333333333333; 

		freelist_head = currentHeader; 
        newLastFreeBlockSize = currentHeader->header.block_size; 
		//result = safeAllocate(currentHeader, padding_amount, space); 

        //for easy return
        lastFreeBlock = currentHeader; 

	}

    return newLastFreeBlockSize; 


}

