#include "holes.h"

void memSIM(FILE *fptr, char* type, int queueTotal, int processTotal, Process *processes, Process *queue){
	int memBytes = 1024, holes = 1, biggerThan, worstRun, bestRun, tp = processTotal, tq = queueTotal, counterVar = 0, j, typeInt, typeForm, memOfProcess = 0, temporary = 0;
	char *swapPriority = malloc(sizeof(char) * memBytes);
	double loads = 0, totalMemUsage = 0, totalProcUsage = 0, totalHoles = 0, currentMemUsage = 0;
	Process *memoryArray = malloc(sizeof(memoryArray) * memBytes);
	memoryArray[0].pid = ' ';
	memoryArray[0].pidSize = memBytes;
	if(!strcmp(type,"first")) {
		typeInt = 0;
		printf("\n\n=============================FIRST FIT==============================\n\n");
	}
	else if(!strcmp(type,"best")) {
		typeInt = 1;
		printf("\n\n=============================BEST FIT==============================\n\n");
	}
	else if(!strcmp(type,"next")) {
		typeInt = 2;
		printf("\n\n=============================NEXT FIT==============================\n\n");
	}
	else if(!strcmp(type,"worst")) {
		typeInt = 3;
		printf("\n\n=============================WORST FIT==============================\n\n");
	}
	if(typeInt == 2) typeInt = 3;
	while(tq > 0){ 
		temporary = holes;
		temporary += memOfProcess;
		worstRun = 0;
		biggerThan = 1;
		bestRun = HIGH;
		for(counterVar = 0; counterVar < temporary; counterVar++){
			if(memoryArray[counterVar].pid == ' '){
				if(memoryArray[counterVar].pidSize >= queue[0].pidSize) {
					biggerThan = 0;
					if(memoryArray[counterVar].pidSize > worstRun) worstRun = counterVar;
					if(memoryArray[counterVar].pidSize < bestRun) bestRun = counterVar;
				}
			}
	
		}
	
		for(counterVar = 0; counterVar < temporary; counterVar++){
			if((memoryArray[counterVar].pid == ' ' && memoryArray[counterVar].pidSize >= queue[0].pidSize && (typeInt == 0 || typeInt == 2)) || (typeInt == 1 && biggerThan == 0) || (typeInt == 3 && biggerThan == 0)){
				if(typeInt == 1) counterVar = bestRun;
				if(typeInt == 3) counterVar = worstRun;

				if(memoryArray[counterVar].pidSize > queue[0].pidSize){
					for(j = (holes + tp); j > counterVar; j--) memoryArray[j] = memoryArray[j - 1];
					memoryArray[counterVar + 1].pidSize -= queue[0].pidSize;
				} else holes--;
				memoryArray[counterVar] = queue[0];
				swapPriority[memOfProcess] = memoryArray[counterVar].pid;
				memOfProcess++;
				biggerThan = 0;
				loads++;
				typeForm = 0;
				for(j = 0; j < (memOfProcess + holes); j++){
					if(memoryArray[j].pid != ' ') typeForm += memoryArray[j].pidSize;
				}
				currentMemUsage = (typeForm * 100) / memBytes;
				totalMemUsage += currentMemUsage;
				totalProcUsage += memOfProcess;
				totalHoles += holes;
				int cumMem = (totalMemUsage / loads);
				printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.0f, cumulative %%mem = %d\n", memOfProcess, holes, currentMemUsage, cumMem);
				
				for(j = 0; j < (tq - 1); j++) queue[j] = queue[j + 1];
				tq--;
				if(typeInt != 2) break;
			}
		}
	
		if(biggerThan){
			temporary = memOfProcess + holes;
			for(counterVar = 0; counterVar < temporary; counterVar++){
				if(swapPriority[0] == memoryArray[counterVar].pid){
					for(j = 0; j < (memOfProcess - 1); j++) swapPriority[j] = swapPriority[j + 1];
					memoryArray[counterVar].swap++;
					if(memoryArray[counterVar].swap < 3){
						queue[tq] = memoryArray[counterVar];
						tq++;
					}
				
					typeForm = 0;
					if(counterVar > 0 && (counterVar + 1) != (memOfProcess + holes)){
						if(' ' == memoryArray[counterVar + 1].pid && ' ' == memoryArray[counterVar - 1].pid){
							memoryArray[counterVar - 1].pidSize += (memoryArray[counterVar].pidSize + memoryArray[counterVar + 1].pidSize);
							holes--;
							typeForm = 1;
						} else if(' ' == memoryArray[counterVar + 1].pid) {
							memoryArray[counterVar].pidSize += memoryArray[counterVar + 1].pidSize;
							memoryArray[counterVar].pid = ' ';
							typeForm = 2;
						} else if(' ' == memoryArray[counterVar - 1].pid) {
							memoryArray[counterVar - 1].pidSize += memoryArray[counterVar].pidSize;
							typeForm = 3;
						} else {
							memoryArray[counterVar].pid = ' ';
							holes++;
						}
					} else if((counterVar + 1) != (memOfProcess + holes)) {
						if(' ' == memoryArray[counterVar + 1].pid){
							memoryArray[counterVar].pidSize += memoryArray[counterVar + 1].pidSize;
							memoryArray[counterVar].pid = ' ';
							typeForm = 2;
						} else {
							memoryArray[counterVar].pid = ' ';
							holes++;
						}
					} else if(counterVar > 0) {
						if(' ' == memoryArray[counterVar - 1].pid) {
							memoryArray[counterVar - 1].pidSize += memoryArray[counterVar].pidSize;
							typeForm = 3;
						} else {
							memoryArray[counterVar].pid = ' ';
							holes++;
						}
					} else {
						memoryArray[counterVar].pid = ' ';
						holes++;
					}
					memOfProcess--;
				
					if(typeForm == 1) for(j = counterVar; j < (memOfProcess + holes); j++) memoryArray[j] = memoryArray[j + 2];
					else if(typeForm == 2) for(j = (counterVar + 1); j < (memOfProcess + holes); j++) memoryArray[j] = memoryArray[j + 1];
					else if(typeForm == 3) for(j = counterVar; j < (memOfProcess + holes); j++) memoryArray[j] = memoryArray[j + 1];
					break;
				}
			}
		}
	}
	printf("Total loads = %.0f, average processes = %.1f, average #holes = %.1f, cumulative %%mem = %d\n", loads, totalProcUsage / loads, totalHoles / loads, (int)(totalMemUsage / loads));

	memoryArray[0].pidSize = memBytes;
	memoryArray[0].pid = ' ';
	for(counterVar = 0; counterVar < tp; counterVar++){
		if(processes[counterVar].pidSize <= memBytes){
			processes[counterVar].swap = 0;
			queue[tq] = processes[counterVar];
			tq++;
		}
	}

	free(memoryArray);
	free(swapPriority);
}

void error() {
	printf("Invalid format/file. \n");
	exit(1);
}
