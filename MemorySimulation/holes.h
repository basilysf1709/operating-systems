#ifndef HEADER_HOLES_FILE
#define HEADER_HOLES_FILE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define HIGH 99999

typedef struct Process{
	char pid;
	int pidSize;
	int swap;
	int totalProcess;
} Process;

typedef struct Total {
	int processTotal;
	int queueTotal;
} Total;

void memSIM(FILE *fptr,char* type,int queueTotal, int processTotal, Process *processes,Process *queue);
void error();

#endif