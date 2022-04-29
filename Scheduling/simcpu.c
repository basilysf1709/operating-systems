#include "simcpu.h"

/**
 * Student Name: Basil Yusuf
 * Student ID: 1158677
 */
void errorPrint();
int p(int number);
void swapBurst(cpuBurst **burstList, int i, int j);
void upwardShift(cpuBurst **burstList, int i);
int leftSideChild(int number);
int rightSideChild(int i);
void downwardShift(cpuBurst **burstList, int size, int i);
void insertElement(cpuBurst **burstList, int *size, int p);
int extractMinimum(cpuBurst **burstList, int *size);
void *getMinimum (cpuBurst **burstList);
void clearProcess(Process *tempProcess);
int onProcessComplete(Process **list, int np);
double averageTurnaround(Process **list, int np);
void printOutput(Process **list, int np);

int main(int argc, char *argv[]) {
    int dCheck = 0, vCheck = 0, rCheck = 0, i, opt;
    while ((opt = getopt(argc, argv, "dvr:")) != -1) {
        switch (opt) {
            case 'd':
                dCheck = 1;
                break;
            case 'v':
                vCheck = 1;
                break;
            case 'r':
                rCheck = atoi(optarg);
                if (rCheck <= 0) {
                    printf("Error: quantum cannot be less than 1\n");
                    exit(0);
                }
                break;
            case '?':
                printf("unknown option: -%c\n", optopt);
                break;
        }
    }
    for (i = optind; i < argc; i++) printf("Unkown argument: %s\n", argv[i]);
    char input[INPUT_SIZE];
    int np, tSwitch, pSwitch, processNo, tmpPNo, noThreads, tNo, tmpTNo, at, nB, burstNumber;
    int totalBursts = 0;
    Process **list;

    if (fgets(input, INPUT_SIZE, stdin) != NULL) {
        if (strlen(input) < 6) errorPrint();
        list = malloc(sizeof(Process *) * np + INPUT_SIZE);
        sscanf(input, "%d %d %d", &np, &tSwitch, &pSwitch);

        for (processNo = 0; processNo < np && fgets(input, INPUT_SIZE, stdin)!=NULL; processNo++) {
            if (strlen(input) < 4) errorPrint();
            sscanf(input, "%d %d", &tmpPNo, &noThreads);
            Process *tempProcess = malloc(sizeof(Process));
            list[processNo] = tempProcess;
            tempProcess->noThreads = noThreads;
            tempProcess->processNo = tmpPNo;
            tempProcess->threadList = malloc(sizeof(Thread*)*noThreads);

            for (tNo = 0; tNo < noThreads && fgets(input, INPUT_SIZE, stdin)!=NULL; tNo++) {
                if (strlen(input) < 6) errorPrint();
                sscanf(input, "%d %d %d", &tmpTNo, &at, &nB);
                Thread *tmp = malloc(sizeof(Thread));
                tmp->tNo = tmpTNo;
                tmp->at = at;
                tmp->numberCPUBurst = nB;
                tmp->done = 0;
                tmp->parent = tempProcess;
                tmp->list = malloc(sizeof(cpuBurst*)*nB);
                tempProcess->threadList[tNo] = tmp;
                
                for (burstNumber = 0; burstNumber < nB && fgets(input, INPUT_SIZE, stdin)!=NULL; burstNumber++) {
                    if (strlen(input) < 4) errorPrint();
                    char tempLine[INPUT_SIZE];
                    strcpy(tempLine, input);
                    char *token = strtok(tempLine, " ");
                    totalBursts++;
                    cpuBurst *tmpCPU = malloc(sizeof(cpuBurst));
                    tmpCPU->burstNumber = burstNumber+1;
                    tmpCPU->timeForCPU = atoi(strtok(NULL, " "));
                    tmpCPU->remTimeForCPU = tmpCPU->timeForCPU;
                    token = strtok(NULL, " ");
                    if (token != NULL) tmpCPU->InputOutputTime = atoi(token);
                    else tmpCPU->InputOutputTime = 0;
                    tmpCPU->done = 0;
                    tmpCPU->parent = tmp;
                    tmp->list[burstNumber] = tmpCPU;
                }
            }
        }
    } else errorPrint();

    cpuBurst **readyQueue = malloc(sizeof(cpuBurst *) * totalBursts);
    int qSIZE = -1, running = 0, totalCStime = 0, time;
    double CPUUtil = 0, averageTurnaroundTime = 0;
    cpuBurst *curr;
    cpuBurst *prev = NULL;
    for (time = 0; onProcessComplete(list, np) == 0; time++) {
        for (int j = 0; j < np; j++) {
            for (int k = 0; k < list[j]->noThreads; k++) {
                if (list[j]->threadList[k]->at == time) {
                    readyQueue[qSIZE+1] = list[j]->threadList[k]->list[0];
                    insertElement(readyQueue, &qSIZE, time);
                    if (vCheck == 1) printf("At time %4d: Thread %d of Process %d moves from new to ready\n", time, k + 1, j + 1);
                }
            }
        }
        if (running == 1) {
            if ((curr->CPUEnterTime + curr->remTimeForCPU) != time && rCheck != 0 && (curr->CPUEnterTime + rCheck) == time) {
                extractMinimum(readyQueue, &qSIZE);
                curr->remTimeForCPU -= rCheck;
                readyQueue[qSIZE + 1] = curr;
                insertElement(readyQueue, &qSIZE, time);
                running = 0;
                if (qSIZE != -1) {
                    cpuBurst *nextBurst = getMinimum(readyQueue);
                    int sTime;
                    if (((Process *)((Thread *)nextBurst->parent)->parent)->processNo != ((Process *)((Thread *)curr->parent)->parent)->processNo) {
                        sTime = pSwitch;
                        if (vCheck == 1)
                            printf("At time %4d: Thread %d of Process %d moves from running to blocked\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    } else if (((Thread *)nextBurst->parent)->tNo != ((Thread *)curr->parent)->tNo){//if it switches to a different thread in the same process
                        sTime = tSwitch;
                        if (vCheck == 1)
                            printf("At time %4d: Thread %d of Process %d moves from running to blocked\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    } else sTime = 0;

                    totalCStime += sTime;
                    int originalTime = time;
                    if (sTime != 0) {
                        for (time = time + 1; time < (originalTime + sTime); time++) {
                            for (int j = 0; j < np; j++) {
                                for (int k = 0; k < list[j]->noThreads; k++) {
                                    if (list[j]->threadList[k]->at == time) {
                                        readyQueue[qSIZE+1] = list[j]->threadList[k]->list[0];
                                        insertElement(readyQueue, &qSIZE, time);
                                        if (vCheck == 1) 
                                            printf("At time %4d: Thread %d of Process %d moves from new to ready\n", time, k + 1, j + 1);
                                    }
                                }
                            }
                        }
                    }
                }
            } else if ((curr->CPUEnterTime + curr->remTimeForCPU) == time) {
                extractMinimum(readyQueue, &qSIZE);
                curr->done = 1;
                running = 0;
                if (curr->burstNumber == ((Thread *)curr->parent)->numberCPUBurst) {
                    ((Thread *)curr->parent)->done = 1;
                    ((Thread *)curr->parent)->endTime = time;
                    ((Thread *)curr->parent)->turnaroundTime = time - ((Thread *)curr->parent)->at;
                    if (vCheck == 1)
                        printf("At time %4d: Thread %d of Process %d moves from running to terminated\n", time + curr->InputOutputTime, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                } else {
                    readyQueue[qSIZE+1] = ((Thread *)curr->parent)->list[curr->burstNumber];
                    insertElement(readyQueue, &qSIZE, time+curr->InputOutputTime);
                }
                if (qSIZE != -1) {
                    cpuBurst *nextBurst = getMinimum(readyQueue);
                    int sTime;
                    if (((Process *)((Thread *)nextBurst->parent)->parent)->processNo != ((Process *)((Thread *)curr->parent)->parent)->processNo) {
                        sTime = pSwitch;
                        if (vCheck == 1 && ((Thread *)curr->parent)->done == 0)
                            printf("At time %4d: Thread %d of Process %d moves from running to blocked\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    } else if (((Thread *)nextBurst->parent)->tNo != ((Thread *)curr->parent)->tNo){
                        sTime = tSwitch;
                        if (vCheck == 1 && ((Thread *)curr->parent)->done == 0)
                            printf("At time %4d: Thread %d of Process %d moves from running to blocked\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    } else sTime = 0;

                    totalCStime += sTime;
                    int originalTime = time;
                    if (sTime != 0) {
                        for (time = time+1; time <(originalTime+sTime); time++) {
                            for (int j = 0; j < np; j++) {
                                for (int k = 0; k < list[j]->noThreads; k++) {
                                    if (list[j]->threadList[k]->at == time) {
                                        readyQueue[qSIZE+1] = list[j]->threadList[k]->list[0];
                                        insertElement(readyQueue, &qSIZE, time);
                                        if (vCheck == 1)  
                                            printf("At time %4d: Thread %d of Process %d moves from new to ready\n", time, k + 1, j + 1);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (running == 0 && qSIZE != -1) {
            curr = getMinimum(readyQueue);
            if (curr->ready <= time) {
                curr->CPUEnterTime = time;
                running = 1;
                if (curr->burstNumber == 1) ((Thread *)curr->parent)->startTime = time;
                if (vCheck == 1 && time == 0) 
                    printf("At time %4d: Thread %d of Process %d moves from ready to running\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                if (vCheck == 1 && prev != NULL && !( ((Thread *)curr->parent)->tNo == ((Thread *)prev->parent)->tNo && ((Process *)((Thread *)curr->parent)->parent)->processNo == ((Process *)((Thread *)prev->parent)->parent)->processNo) ) {
                    if (curr->burstNumber != 1 || (curr->burstNumber ==1 && curr->timeForCPU != curr->remTimeForCPU)) 
                        printf("At time %4d: Thread %d of Process %d moves from blocked to running\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    else 
                        printf("At time %4d: Thread %d of Process %d moves from ready to running\n", time, ((Thread *)curr->parent)->tNo, ((Process *)((Thread *)curr->parent)->parent)->processNo);
                    
                }
            }
            prev = getMinimum(readyQueue);
            if (running == 0) {
                totalCStime++;
            }
            
        }

    }
    free(readyQueue);
    time--;
    averageTurnaroundTime = averageTurnaround(list, np);
    CPUUtil = ((double)time-(double)totalCStime)*INPUT_SIZE/(double)time;
    if (rCheck == 0) printf("\nFCFS Scheduling\n");
    else printf("\nRound Robin Scheduling (quantum = %d units)\n", rCheck);
    printf("Total Time required is %d units\nAverage Turnaround Time is %.1lf units\n", time , averageTurnaroundTime);
    printf("CPU Utilization is %.1lf%%\n", CPUUtil);
    if (dCheck == 1 || vCheck == 1) printOutput(list, np);
    for (i = 0; i < np; i++) clearProcess(list[i]);
    free(list);
    return 0;
}

int p(int number) {
    return (number - 1) / 2;
}
void swapBurst(cpuBurst **burstList, int i, int j) {
    cpuBurst *temp = burstList[i];
    burstList[i] = burstList[j];
    burstList[j] = temp;
}
void upwardShift(cpuBurst **burstList, int i) {
    while (i > 0 && burstList[p(i)]->ready > burstList[i]->ready) {
        swapBurst(burstList, p(i), i);
        i = p(i);
    }
}
int leftSideChild(int number) {
    return (1 + (2 * number));
}
int rightSideChild(int i) {
    return (2 + (2 * i));
}
void downwardShift(cpuBurst **list, int size, int i) {
    int maxIndex = i, l = leftSideChild(i),r = rightSideChild(i);
    if (l <= size && list[l]->ready < list[maxIndex]->ready) maxIndex = l;
    if (r <= size && list[r]->ready < list[maxIndex]->ready) maxIndex = r;
    if (i != maxIndex) {
        swapBurst(list, i, maxIndex);
        downwardShift(list, size, maxIndex);
    }
}
int extractMinimum(cpuBurst **list, int *s) {
    int result = list[0]->ready;
    list[0] = list[*s];
    *s = *s - 1;
    downwardShift(list, *s, 0);
    return result;
}
void *getMinimum (cpuBurst **burstList) {
    return burstList[0];
}
void insertElement(cpuBurst **list, int *sizeOfElement, int a) {
    *sizeOfElement = *sizeOfElement + 1;
    list[*sizeOfElement]->ready = a;
    upwardShift(list, *sizeOfElement);
}
void clearProcess(Process *p) {
    int i, j;
    for (i = 0; i < p->noThreads; i++) {
        Thread *tmp = p->threadList[i];
        for (j = 0; j < tmp->numberCPUBurst; j++) {
            free(tmp->list[j]);
        }
        free(tmp->list);
        free(tmp);
    }
    free(p->threadList);
    free(p);
}
int onProcessComplete(Process **p, int np) {
    for (int i = 0; i < np; i++) {
        Process *tempProcess = p[i];
        for (int j = 0; j < tempProcess->noThreads; j++) {
            Thread *tmp = tempProcess->threadList[j];
            if (tmp->done == 0) {
                return 0;
            }
        }
    }
    return 1;
}
double averageTurnaround(Process **p, int np) {
    int totalTime = 0, earliestArrTime = 9999, endTime = -1;
    for (int i = 0; i < np; i++) {
        Process *tempProcess = p[i];
        for (int j = 0; j < tempProcess->noThreads; j++) {
            Thread *tmp = tempProcess->threadList[j];
            if(j == 0) {
                earliestArrTime = tmp->at;
                endTime = tmp->endTime;
            }else {
                if (tmp->at < earliestArrTime) earliestArrTime = tmp->at;
                if (tmp->endTime > endTime) endTime = tmp->endTime;
            }
        }
        totalTime += (endTime - earliestArrTime);
    }
    return (double)totalTime/(double)np;
}
void printOutput(Process **list, int np) {
    int time, InputOutputTime;
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < list[i]->noThreads; j++) {
            time = 0;
            InputOutputTime = 0;
            Thread *tmp = list[i]->threadList[j];
            for (int k = 0; k < tmp->numberCPUBurst; k++) {
                time += tmp->list[k]->timeForCPU;
                InputOutputTime += tmp->list[k]->InputOutputTime;
            }
            printf("Thread %d of Process %d:\n", tmp->tNo, list[i]->processNo);
            printf("  arrival time: %d\n", tmp->at);
            printf("  service time: %d units, I/O time: %d units,", time, InputOutputTime);
            printf(" turnaround time: %d units, finish time: %d units\n", tmp->turnaroundTime, tmp->endTime);
        }
    }
}
void errorPrint(){
    printf("Error: Input file is invalid. Please Check\n");
    exit(0);
}