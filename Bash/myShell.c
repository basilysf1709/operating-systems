#include "myShell.h"

/**
 * Name: Basil Iqbal Yusuf
 * Student Id: 1158677
 * Course: 3110
 * Email: byusuf@uoguelph.ca
*/

int main () {
    struct sigaction sa;
    sa.sa_handler = &handler;
    char** argv;
    char* input = malloc(sizeof(char) * WORD_SIZE);
    int status;
    int inc = 0;
    pid_t pid, pid2;
    char* wd = malloc(sizeof(char) * WORD_SIZE);
    char* historyFile = malloc(sizeof(char) * WORD_SIZE);
    char* profileFile = malloc(sizeof(char) * WORD_SIZE);
    int historyCount = 0;
    char histline[WORD_SIZE];
    char c;
    int i;
    char home[WORD_SIZE];
    strcpy(wd, getcwd(wd,WORD_SIZE));
    strcpy(home, getenv("HOME")); 

    char myPath[WORD_SIZE];
    char path[WORD_SIZE];
    char printPATH[WORD_SIZE];
    strcpy(myPath, "/bin/");
    strcpy(printPATH, myPath);

    strcpy(historyFile, home); // change this to wd to test it in the working directory
    strcat(historyFile, "/.CIS3110_history");
    FILE *historyFilePtr = fopen(historyFile, "a+");
    if (historyFilePtr == NULL) {
        perror("histfileptr");
    }
    fseek(historyFilePtr, 0, SEEK_END);
    if (ftell(historyFilePtr)==0) {
        historyCount = 0;
    } else {
        fseek(historyFilePtr, 0, SEEK_SET);
        for (c = getc(historyFilePtr); c!=EOF; c = getc(historyFilePtr)) {
            if (c == '\n') historyCount++;
        }
    }

    strcpy(profileFile, home); // change this to wd to test it in the working directory
    strcat(profileFile, "/.CIS3110_profile");
    FILE *profileFilePtr = fopen(profileFile, "r");
    if (profileFilePtr == NULL) {
        printf("File did not work.");
    }
    fscanf(profileFilePtr, "%s\n%s\n%s\n", printPATH, home, historyFile);
    fclose(profileFilePtr);


    //Parent Loop
    while(1) {
        errno = 0;
        printf("%s> ", wd); 
        input[0] = 0;
        scanf("%[^\n]", input);
        getchar();
        strcpy(path, myPath);

        if(strcmp(input, "\n") != 0 && input[0] != '\0'){
            fseek(historyFilePtr, 0, SEEK_END);
            fprintf(historyFilePtr, " %d  %s\n", ++historyCount, input);
        }

        if(waitpid(pid, NULL, WNOHANG) > 0) printf("[%d]+ %d done        \n", inc, pid);

        // if exit do this
        if (!strcmp(input, "exit")) { 
            exitShell(); 
        }

        //Implementing piping with redirection
        if (strstr(input, "|") != NULL && (strstr(input, "<") != NULL || strstr(input, ">") != NULL)) {
            pipeAndRedirection(input, path);
		}
        //Implementing piping without redirection
        else if (strstr(input, "|") != NULL) {
            pipeWithoutRedirection(input, path);
        }
        else if (strstr(input, "<") != NULL) {
			pid = fork();
			if (pid == -1) perror("fork");
			else if (pid == 0) {
                argv = parseInput(input);
                strcat(path, argv[0]);
                readShell(argv, input, path);
            }
			else {
				waitpid(pid, NULL, 0);
			}
		}// other stuff that is here
        else if (strstr(input, ">") != NULL) {
			pid = fork();
			if (pid == -1) perror("fork");
			else if (pid == 0) {
                argv = parseInput(input);
                strcat(path, argv[0]);
                writeShell(argv, input, path);
			}
			else {
				waitpid(pid, NULL, 0);
			}
		}   
        //backgroundProcess     
        else if (input[strlen(input)-1] == '&') {
            inc++;
            pid = fork();

            argv = parseInput(input);
            strcat(path, argv[0]);
            backgroundProcess(pid, inc, argv, input, path);
        } 
        // check
        else if(strchr(input, '>')) {
            char *p = strrchr(input, ' ');
            if (p && *(p + 1)) printf("%s", p);
        } 
        //making cd work
        else if(strstr(input, "cd") != NULL){
            argv = parseInput(input);
            if(!strcmp(input, "cd")) chdir(home);
            else chdir(argv[1]);
            strcpy(wd, getcwd(wd,WORD_SIZE));
        }
        else if(strstr(input, "history") != NULL){
            argv = parseInput(input);
            if (argv[1] != NULL && strcmp(argv[1] , "-c")== 0) {
                historyFilePtr = freopen(historyFile, "w+", historyFilePtr);
                historyCount = 0;
            } 
            else if (argv[1] !=NULL && atoi(argv[1] )!=0) {
                fseek(historyFilePtr, -1, SEEK_END);
                for (c = getc(historyFilePtr), i=0; i!=atoi(argv[1] )+1 && i<=historyCount; c = getc(historyFilePtr)) {
                    if (c == '\n') i++;
                    fseek(historyFilePtr, -2, SEEK_CUR);
                }
                fseek(historyFilePtr, 1, SEEK_CUR);
                if (i==historyCount) {
                    fseek(historyFilePtr, 0, SEEK_SET);
                }
                while (fgets(histline, 1000, historyFilePtr)!=NULL) {
                    printf("%s", histline);
                }
            }else {
                fseek(historyFilePtr, 0, SEEK_SET);
                while (fgets(histline, 1000, historyFilePtr)!=NULL) {
                    printf("%s", histline);
                }
            }
        }
        else if(strstr(input, "echo") && strstr(input, "$myPATH")){
            pid2 = fork();
            if(pid2 == 0) { 
                argv = parseInput(input);
                strcpy(argv[1], printPATH);
                strcat(path, argv[0]);
                execv(path, argv);

                if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
            }
            else wait(NULL);
        }
        else if(strstr(input, "echo") && strstr(input, "$myHISTFILE")){
            pid2 = fork();
            if(pid2 == 0) { 
                argv = parseInput(input);
                strcpy(argv[1], historyFile);
                strcat(path, argv[0]);
                execv(path, argv);

                if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
            }
            else wait(NULL);
        }
        else if(strstr(input, "echo") && strstr(input, "$myHOME")){
            pid2 = fork();
            if(pid2 == 0) { 
                argv = parseInput(input);
                strcpy(argv[1], home);
                strcat(path, argv[0]);
                execv(path, argv);

                if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
            }
            else wait(NULL);
        }
        else if(strstr(input, "export") && strstr(input, "=")){
            argv = parseInput(input);
            if(strstr(argv[2], "$")){
                if(strstr(argv[2], "$myPATH") != NULL) strcpy(printPATH, str_replace(argv[2], "$myPATH", printPATH));
                if(strstr(argv[2], "$myHOME") != NULL) strcpy(printPATH, str_replace(argv[2], "$myHOME", home));
            } else {
                strcpy(printPATH, argv[2]);
            }

        }
        else {
            pid2 = fork();
            if(pid2 == 0) { 
                argv = parseInput(input);
                strcat(path, argv[0]); 
                execv(path, argv);

                if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
            }
            else wait(NULL);
        }
    }

    free(profileFile);
    free(historyFile);
    free(input);
    free(wd);
    fclose(historyFilePtr);
}

char** parseInput (char *string) {
    int pos = 0;
    char** array;
    char *words;

    array = malloc(sizeof(char*) * ARRAY_SIZE);
    words = malloc(sizeof(char) * WORD_SIZE);
    for (int j = 0; j < WORD_SIZE; j++) array[j] = malloc(sizeof(char) * WORD_SIZE);

    strcpy(words, string);
    char* split = strtok(words, DELIM);
    while (split) {
        strcpy(array[pos], split);
        split = strtok(NULL, DELIM);
        pos++;
    }
    array[pos] = NULL;
    return array;
}

void exitShell() {
    printf("myShell terminating…\n\n");
    kill(getpid(), SIGQUIT);
    exit(1);
}

void backgroundProcess(pid_t pid, int inc, char** argv, char* input, char* path) {
    if(pid == 0){
        input[strlen(input)-1] = '\0';
        argv = parseInput(input);
        execv(path, argv);
        exit(1);
    }else {
        printf("[%d]  %d\n", inc, pid);
    }
}

void readShell(char** argv, char* input, char* path) {
    char *split = strtok(input, "<");
    char commandName[20];
    strcpy(commandName, split);
    split = strtok(NULL, "< ");
    argv = parseInput(commandName);
    freopen(split, "r", stdin);
    execv(path, argv);

    if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
}

void writeShell(char **argv, char* input, char* path) {
    char *split = strtok(input, ">");
    char commandName[20];
    strcpy(commandName, split);
    split = strtok(NULL, "> ");
    argv = parseInput(commandName);
    freopen(split, "w", stdout);
    execv(path, argv);

    if(errno == ENOENT) printf("%s: command not found\n", argv[0]);
}

char *str_replace(char *originalString, char *rep, char *withString) {
    char *returnedString; 
    char *ins; 
    char *tempVar;   
    int lengthOfReplace, lengthOfWidth, lengthOfFront, counter; 
    if (!originalString || !rep) return NULL;
    lengthOfReplace = strlen(rep);
    if (lengthOfReplace == 0) return NULL;
    if (!withString) withString = "";
    lengthOfWidth = strlen(withString);
    ins = originalString;
    for (counter = 0; tempVar = strstr(ins, rep); ++counter) ins = tempVar + lengthOfReplace;
    tempVar = returnedString = malloc(strlen(originalString) + (lengthOfWidth - lengthOfReplace) * counter + 1);
    if (!returnedString) return NULL;

    for(; counter > 0; --counter){
        ins = strstr(originalString, rep);
        lengthOfFront = ins - originalString;
        tempVar = strncpy(tempVar, originalString, lengthOfFront) + lengthOfFront;
        tempVar = strcpy(tempVar, withString) + lengthOfWidth;
        originalString += lengthOfFront + lengthOfReplace; 
    }
    strcpy(tempVar, originalString);
    return returnedString;
}

void pipeAndRedirection(char* input, char* path){
    char firstInput[WORD_SIZE];
    char secondInput[WORD_SIZE];
    char*split = strtok(input, "|");
    strcpy(firstInput, split);
    split = strtok(NULL, "|");
    strcpy(secondInput, split);
    char**firstArg = parseInput(firstInput);
    char**secondArg = parseInput(secondInput);
    int FileDescriptors[2];
    pipe(FileDescriptors);
    pid_t id1= fork();
    if (id1 == -1) perror("fork");
    else if (id1 == 0) {
        dup2(FileDescriptors[1], STDOUT_FILENO);
        close(FileDescriptors[1]);
        close(FileDescriptors[0]); 
        if(strstr(firstInput, "<")!=NULL){
            char copyString[WORD_SIZE];
            strcpy(copyString, firstInput);
            char*  split = strtok(firstInput, "<");
            char command[WORD_SIZE];
            strcpy(command, split);
            split = strtok(NULL, "< ");
            firstArg = parseInput(command);
            freopen(split, "r", stdin);
        }
        else firstArg = parseInput(firstInput);

        strcat(path, firstArg[0]);
        execv(path, firstArg);
    }

    pid_t id2 = fork();
    if (id2 == -1) {
        perror("fork");
    }
    else if (id2 == 0) {
        dup2(FileDescriptors[0], STDIN_FILENO);
        close(FileDescriptors[0]);
        close(FileDescriptors[1]);
        if(strstr(secondInput, ">")!=NULL){
            char copyString[WORD_SIZE];
            strcpy(copyString, secondInput);
            char*  split = strtok(secondInput, ">");
            char command[WORD_SIZE];
            strcpy(command, split);
            split = strtok(NULL, "> ");
            secondArg = parseInput(command);
            if(strstr(copyString, ">>")!=NULL)freopen(split, "a", stdout);
            else freopen(split, "w", stdout);
        }
        else secondArg = parseInput(secondInput);

        strcat(path, secondArg[0]);
        execv(path, secondArg);
    }
    close(FileDescriptors[0]);
    close(FileDescriptors[1]);
    waitpid(id1, NULL, 0);
    waitpid(id2, NULL, 0);
}

void pipeWithoutRedirection(char* input, char* path){
    char firstInput[WORD_SIZE];
    char secondInput[WORD_SIZE];
    char*split = strtok(input, "|");
    strcpy(firstInput, split);
    split = strtok(NULL, "|");
    strcpy(secondInput, split);
    char**firstArg = parseInput(firstInput);
    char**secondArg = parseInput(secondInput);
    int FileDescriptors[2];
    pipe(FileDescriptors);
    //for the writing 
    pid_t id1= fork();
    if (id1 == -1) {
        perror("fork");
    }
    else if (id1 == 0) {
        dup2(FileDescriptors[1], STDOUT_FILENO);
        close(FileDescriptors[1]);
        close(FileDescriptors[0]);
        strcat(path, firstArg[0]);
        execv(path, firstArg);
    }

    pid_t id2 = fork();
    if (id2 == -1) {
        perror("fork");
    }
    else if (id2 == 0) {
        dup2(FileDescriptors[0], STDIN_FILENO);
        close(FileDescriptors[0]);
        close(FileDescriptors[1]);
        //secondArg = parseInput(secondInput);
        
        strcat(path, secondArg[0]);
        execv(path, secondArg);
    }
    close(FileDescriptors[0]);
    close(FileDescriptors[1]);
    waitpid(id1, NULL, 0);
    waitpid(id2, NULL, 0);
}