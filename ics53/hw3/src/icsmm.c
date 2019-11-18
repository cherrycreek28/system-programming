/**
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 * If you want to make helper functions, put them in helpers.c
 */
#include "icsmm.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

ics_free_header *freelist_head = NULL;
//void* PROLOGUE; 
//void* EPILOGUE;
//void* pL = NULL; 
//void* eL = NULL; 

#define HEADER_SIZE  8
#define FOOTER_SIZE  8 
#define DOUBLE_POINTER_SIZE  16 
#define PROLOGUE_SIZE 8
#define EPILOGUE_SIZE 8

void* prologueAddress = 0; 

void *ics_malloc(size_t size) { 

	//the two exceptions that might happen
	if (size == 0)
	{
		errno = EINVAL; 
		return NULL; 
	}

	if (size >= 4096*4)
	{
		errno = ENOMEM; 
		return NULL; 
	}

	//some set-ups and declarations
	void* result; 
	ics_free_header* lastHeader = NULL; 
	int padding_amount = 0; 
	int space = spaceNeeded(size, &padding_amount); 

	//figure out if the heap is initiated or not
	if (prologueAddress == 0)
	{
		prologueAddress = ics_inc_brk();
		int sizeofLargeChunk = (void*)ics_get_brk() - (void*)prologueAddress;

		//allocated bit is 1
		ics_header* pL = (ics_header*)(prologueAddress); 
		pL->block_size = 0; 
		ics_header* eL = (ics_header*)(ics_get_brk() - 8); 
		eL->block_size = 0; 

		pL->block_size++; 
		eL->block_size++; 
		
		void* currentHeaderAddress = (void*)pL+8; 
		ics_free_header* currentHeader = (ics_free_header*)(currentHeaderAddress);
		currentHeader->header.block_size = sizeofLargeChunk - PROLOGUE_SIZE - EPILOGUE_SIZE;
		currentHeader->header.unused = 0x1999999999999; 

		currentHeader->next = NULL; 
		currentHeader->prev = NULL;
		
		void* currentFooterAddress = (void*)eL-8; 
		ics_footer* currentFooter = (ics_footer*)(currentFooterAddress);
		currentFooter->block_size = sizeofLargeChunk - PROLOGUE_SIZE - EPILOGUE_SIZE;
		currentFooter->padding_amount = 0; 
		currentFooter->unused = 0x333333333333; 

		freelist_head = currentHeader; 
	}

	//find the first available free block
	ics_free_header* tempHeader = freelist_head; 
	while (tempHeader != NULL)
	{
		
		if (tempHeader->header.block_size >= space)
		{
			result = safeAllocate(tempHeader, padding_amount, space); 
			return result; 
		}

		lastHeader = tempHeader; 
		tempHeader = tempHeader->next; 
	}

	//means we need to ask for more page
	int moreSpace = morePage(lastHeader); 

	//to ask for pages until the request can be satisfied
	while(moreSpace<space)
	{
		moreSpace = morePage(lastHeader); 
	}

	result = safeAllocate(lastHeader, padding_amount, space); 
	return result;
}

//malloc()+copy()+free()
void *ics_realloc(void *ptr, size_t size)
{
	int padding_amount; 
	int space = spaceNeeded(size, &padding_amount); 

	
	if (size == 0)
	{
		ics_free(ptr); 
		return NULL; 
	}

	if (size >= 4096*4)
	{
		errno = ENOMEM; 
		return NULL; 
	}

	void* result; 
	void* temp = ptr - HEADER_SIZE;
	ics_free_header* tempHeader = (ics_free_header* )temp; 

	//when the request is asking for larger space
	if (size>=tempHeader->header.block_size - HEADER_SIZE - FOOTER_SIZE)
	{

		result = ics_malloc(size);
		int i = 0; 

		for (; i<tempHeader->header.block_size; i++)
		{
			*(char* )(result+i) = *(char* )(ptr+i); 
		}

		//to check if the pointer is valid or not in free()
		int check; 
		if ((check = ics_free(ptr)) == -1)
		{
			return NULL;
		}
	}

	//when the request is asking for smaller space
	else
	{
		void* ifShrink = shrinkAllocatedBlock(tempHeader, padding_amount, space); 
		
		//if the Shrink func returns NULL: no need to shrink
		//else the returned pointer "ifShrink" is the header address of the nextHeader
		//currentHeader(kept)|nextHeader(eliminated)
		if (ifShrink != NULL)
		{
			/*int check; 
			if ((check = ics_free(ifShrink)) == -1)
			{
				return NULL; 
			}*/
			ics_free(ifShrink);  
		}

		result = ptr; 

	}

	return result; 
}


/*
the ics_free function frees the allocated block
*/
int ics_free(void *ptr)
{

	ptr -= HEADER_SIZE; 
	ics_free_header* currentHeader = (ics_free_header*)(ptr); 

	//first do some checks to ensure the pointer is valid; 
	if (ptr<prologueAddress || ptr>ics_get_brk())
	{
		errno = EINVAL; 
		return -1; 
	}

	if (currentHeader->header.unused != 0x1999999999999)
	{
		errno = EINVAL; 
		return -1; 
	}

	if (currentHeader->header.block_size%2 == 0)
	{
		errno = EINVAL; 
		return -1; 
	}
/*
	if ((unsigned long)currentHeader%8 != 0)
	{
		errno = EINVAL; 
		return -1; 
	}
*/
	//try to figure out the correct block size with MASK
	int mask = 0xfffc; 

	void* currentFooterAddress = (void*)currentHeader + ((currentHeader->header.block_size)&mask) -FOOTER_SIZE; 
	ics_footer* currentFooter = (ics_footer*)(currentFooterAddress); 

	if (currentFooter->block_size%2 == 0)
	{
		errno = EINVAL; 
		return -1; 
	}

	if (currentFooter->block_size != currentHeader->header.block_size)
	{
		errno = EINVAL; 
		return -1; 
	}

	if (currentFooter->unused != 0x333333333333)
	{
		errno = EINVAL; 
		return -1; 
	}

	//set the allocated bit to free; 
	currentHeader->header.block_size&=mask; 
	currentFooter->block_size&=mask;


	ics_free_header* tempHeader = freelist_head; 
	ics_free_header* lastHeader = NULL; 
	
	//try to figure out if the current Header can be coalesced with the previous one
	//if the free list isn't NULL; 

	//int flag = 0; 

	while (tempHeader != NULL)
	{
		//find the best position in the free list, and insert;
		if (currentHeader<tempHeader)
		{
			ics_free_header* prevHeader = tempHeader->prev; 
			//flag = 1; 
			if (prevHeader != NULL)
			{
				prevHeader->next = currentHeader; 
				currentHeader->prev = prevHeader; 
				currentHeader->next = tempHeader; 
				tempHeader->prev = currentHeader; 
			}

			else
			{
				currentHeader->next = tempHeader; 
				tempHeader->prev = currentHeader; 
				currentHeader->prev = NULL; 
				freelist_head = currentHeader; 
			}

			break; 
		}
		
		lastHeader = tempHeader; 
		tempHeader = tempHeader->next; 
	}

	if (tempHeader == NULL)
	{
		if (lastHeader == NULL)
		{
			freelist_head = currentHeader; 
		}

		else 
		{
			lastHeader->next = currentHeader; 
			currentHeader->prev = lastHeader; 
		}

	}

	ics_free_header* prevHeader = currentHeader->prev; 
	
	if (prevHeader != NULL)
	{
		//merge with the prev
		if ((void*)prevHeader+prevHeader->header.block_size == currentHeader)
		{
			prevHeader->header.block_size+=currentHeader->header.block_size; 
			prevHeader->next = currentHeader->next; 
			if (currentHeader->next != NULL)
			{
				currentHeader->next->prev = prevHeader; 
			}
			currentFooter->block_size = prevHeader->header.block_size; 
			currentHeader = prevHeader; 
		}
	}
	
	ics_free_header* nextHeader = currentHeader->next; 
	if (nextHeader != NULL)
	{
		//merge with the next
		if ((void*)currentHeader+currentHeader->header.block_size == nextHeader)
		{
			currentHeader->header.block_size+=nextHeader->header.block_size; 
			currentHeader->next = nextHeader->next; 

			if (currentHeader->next != NULL)
			{
				currentHeader->next->prev = currentHeader; 
			}

			ics_footer* nextFooter = (ics_footer*) ((void*)nextHeader + nextHeader->header.block_size-FOOTER_SIZE); 
			nextFooter->block_size = currentHeader->header.block_size; 
		}
	
	}
	
	return 0; 
}



