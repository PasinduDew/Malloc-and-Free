#ifndef MYMALLOC_H
#define MYMALLOC_H

typedef struct header{
	char status;
	int size;
	struct header *next;
}header;

void *MyMalloc(size_t);
void MyFree(void *);
void viewMemory(void);

#endif
