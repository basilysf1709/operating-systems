CC = gcc
CFLAGS = -std=gnu99 -Wpedantic -g

all: myShell

myShell.o: myShell.c myShell.h
	$(CC) $(CFLAGS) -c myShell.c -o myShell.o

myShell: myShell.o
	$(CC) $(CFLAGS) -o myShell myShell.o

clean:
	rm *.o myShell