CC = gcc
CFLAGS = -g -Wall
RM = /bin/rm -f

all : main

mymalloc.o : mymalloc.h mymalloc.c
	${CC} ${CFLAGS} -c mymalloc.c

main.o : mymalloc.h main.c
	${CC} ${CFLAGS} -c main.c

 main : main.o mymalloc.o
	${CC} ${CFLAGS} -o main main.o mymalloc.o

clean :
	${RM} *.o main
