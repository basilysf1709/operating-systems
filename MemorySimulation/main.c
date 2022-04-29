#include "holes.h"

int main(int argc, char **argv){
	Total *t = malloc(sizeof(Total));
	t->processTotal = 0;
	t->queueTotal = 0;
	Process *processes = malloc(sizeof(Process));
	Process *queue = malloc(sizeof(Process));
	int memorySize = 1024;


	FILE *fptr = fopen(argv[1], "r");
	if(fptr == NULL && argc < 2) error();
	int count = 0;
	while(!feof(fptr)) {
		processes = realloc(processes, sizeof(Process) * (count + 1));
		fscanf(fptr, "%c %d\n", &processes[count].pid, &processes[count].pidSize);
		if(processes[count].pidSize <= memorySize) {
			queue = realloc(queue, sizeof(Process) * (count + 1));
			processes[count].swap = 0;
			queue[t->queueTotal] = processes[count];
			t->queueTotal++;
		}
		else processes[count].swap = 3;

		count++;
		t->processTotal++;
	}
	if(!strcmp(argv[2],"first")) memSIM(fptr, "first", t->queueTotal, t->processTotal, processes, queue);
	if(!strcmp(argv[2],"best")) memSIM(fptr, "best", t->queueTotal, t->processTotal, processes, queue);
	if(!strcmp(argv[2],"next")) memSIM(fptr, "next", t->queueTotal, t->processTotal, processes, queue);
	if(!strcmp(argv[2],"worst")) memSIM(fptr, "worst", t->queueTotal, t->processTotal, processes, queue);

	free(queue);
	free(processes);
	free(t);
	fclose(fptr);
	return 0;
}