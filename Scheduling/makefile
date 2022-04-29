CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: simcpu

simcpu.o: simcpu.c simcpu.h
	$(CC) $(CFLAGS) -c simcpu.c -o simcpu.o

simcpu: simcpu.o simcpu.c simcpu.h
	$(CC) $(CFLAGS) -o simcpu simcpu.o

clean:
	rm *.o simcpu