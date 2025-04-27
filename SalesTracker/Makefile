CC = gcc
CFLAGS = -g -Wall -std=c99

fundraiser: fundraiser.c input.o group.o

input.o: input.c input.h 

group.o: group.c group.h

clean:
	rm -f *.o
	rm -f fundraiser
