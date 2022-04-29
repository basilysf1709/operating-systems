#ifndef SIMCPU_h
#define SIMCPU_H

#define INPUT_SIZE 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef struct {
    int burstNumber;
    int timeForCPU;
    int remTimeForCPU;
    int InputOutputTime;
    int ready;
    int CPUEnterTime;
    int done;
    void *parent;
}cpuBurst;

typedef struct {
    int tNo;
    int at;
    int numberCPUBurst;
    int startTime;
    int endTime;
    int turnaroundTime;
    int done;
    void *parent;
    cpuBurst **list;
}Thread;

typedef struct {
    int processNo;
    int noThreads;
    Thread **threadList;
}Process;

#endif