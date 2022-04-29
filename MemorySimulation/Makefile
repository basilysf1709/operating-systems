CC=gcc
CFLAGS=-std=gnu99 -Wall -Wpedantic

hole: main.o holes.o
	$(CC) $(CFLAGS) main.o holes.o -o hole

main.o: main.c holes.h
	$(CC) $(CFLAGS) -c main.c -o main.o

holes.o: holes.c holes.h
	$(CC) $(CFLAGS) -c holes.c -o holes.o

clean:
	rm *.o hole

