#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "mymalloc.h"


static char memory[25000]; //This Array Act As the RAM or the Memomory
static int init_flag = 0;


void mergeFree(header *);  //Merge Free Blocks of Memory to Build Larger Free Memory Blocks



void *MyMalloc(size_t size){
	
	if(size <= 0){
		return NULL;
	}
	
	if(init_flag == 0){
		printf("\nInitializing The Memory");
		((header *)(memory)) -> status = 'f';
		((header *)(memory)) -> size = 25000 - sizeof(header);
		((header *)(memory)) -> next = NULL;
		init_flag = 1;
		
		
	}
	
	header *ptr = (header *)(memory);
	int reqSize, prevSize;
	header * prevNext;
	void *retPtr;
	reqSize = size + sizeof(header);
	while(ptr != NULL){
		
		if(ptr -> status == 'f'){
			if(ptr -> size == size){
				ptr -> status = 'a';
			}
			else if(ptr -> size > 2 * reqSize - size){ //If the size of the block is greater than required size we have to split it into two blocks. Therefore, we need two headers. 2 * reqSize - size = size + 2 * sizeof(header)
				//printf("\nI`m Here");
				//Saving Previous Data
				prevSize = ptr -> size;
				prevNext = ptr -> next;
				
				//Allocating 
				ptr -> status = 'a';
				ptr -> size = size;
				retPtr = (void *)((char *)ptr + sizeof(header));
				ptr -> next = (header *)((char *)ptr + size + sizeof(header));
				
				//Make the remaining memory as the free block
				//Creating the header of the New Block
				(ptr -> next) -> status = 'f';
				(ptr -> next) -> size = prevSize - sizeof(header) - size;
				(ptr -> next) -> next = prevNext;
				
				return retPtr; //Returning the Pointer
			}
			else{
				return NULL;
			}
			
			
		}
		ptr = ptr -> next;
	}
	
}



void MyFree(void *freePtr){
	header *ptr = (header *)((char *)freePtr - sizeof(header));
	
	if(ptr -> status == 'a'){
		ptr -> status = 'f';
		int i;
		
		//Clearing the Content of the Memory Block
		for(i = 0; i < ptr -> size; i++){
			*((char *)freePtr + i) = '\0';
		}
		
	}
	
	//Mergeing Adjacency Free Blocks to form Larger Blocks
	mergeFree((header *)(memory));
	
}

void mergeFree(header *startPtr){
	header *ptr, *nextPtr;
	ptr = startPtr;
	
	while(ptr != NULL){
		nextPtr = ptr -> next;
		if(nextPtr != NULL){
			//printf("\nI`m inside the Loop");
			if(ptr -> status == 'f' && nextPtr -> status == 'f'){

				ptr -> next = nextPtr -> next;
				ptr -> size = nextPtr -> size + ptr -> size + sizeof(header); 
			}
			else{
				ptr = ptr -> next;
			}
			
		}
		else{
			break;
		}
		
		
	}
	
}

void viewMemory(){
	
	header *ptr = (header *)(memory);
	char str[10];
	int blockNum = 0;
	printf("\n");
	printf("\nSnap Shot of the Memory");
		printf("\n ___________________________________________________________________________________________");
		printf("\n|     Block No.     |         Status       |     Size (Bytes)    |     Returned Pointer     |");
		printf("\n|___________________|______________________|_____________________|__________________________|");
//		printf("\n|-------------------|");
	while(ptr != NULL){
		if(ptr -> status == 'a'){
			strcpy(str, "Allocated");
			
		}
		else if(ptr -> status == 'f'){
			strcpy(str, "Free");
			
		}
		blockNum++;
		printf("\n|        %3d        |       %-9s      |        %5d        |          %p        |", blockNum, str, ptr -> size, (char *)ptr + sizeof(header) );
			
		ptr = ptr -> next;
	}
	printf("\n|___________________|______________________|_____________________|__________________________|");
}

