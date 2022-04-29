#ifndef MYSHELL_H
#define MYSHELL_H

#define _POSIX_SOURCE
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define ARRAY_SIZE 1024
#define WORD_SIZE 256
#define FILE_SIZE 2048
#define DELIM " \t\r\n\a="

//parsing input
char** parseInput (char *string);
//exiting shell
void exitShell();
// executing the & command
void backgroundProcess(pid_t pid, int inc, char** argv, char* input, char* path);
// input redirection
void readShell(char **argv, char* input, char* path);
//output redirection
void writeShell(char **argv, char* input, char* path);
//changing directory
char* changeDirectory(char* wd);
//handler
void handler() { 
    printf("> ");
    fflush(stdout);
}
char *str_replace(char *orig, char *rep, char *with);
void pipeAndRedirection(char* input, char* path);
void pipeWithoutRedirection(char* input, char* path);

#endif
