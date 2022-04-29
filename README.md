# Operating Systems  ***Creating a shell***

**Name:** Basil Yusuf

**Course:** Operating Systems (CIS*3110)

**Date:**  6th February, 2021

**Version:** 1.0 

## Objective

In this assignment you are to implement a simple UNIX shell program. A shell is simply a 
program that conveniently allows you to run other programs. Read up on the bash shell to see 
what it does.

## Specification

Your shell must support the functions described below. The functions are grouped into 3 sets. 
You may develop the functions in any order, but they are grouped in order of difficulty. It 
would be prudent to get the simpler functions working first. With each function there are some 
suggestions for POSIX system calls that may help with the construction of the function. These 
suggestions are not exhaustive, nor do you necessarily need to use them. They are there to 
stimulate your thinking.

## Assumptions and Limitations
 - We are assuming that the default path is /bin/
 - There's an assumption that & will always be at the end
 - history command does not work with > or <

## User Guide
 - After being in the correct directory with the "makefile" and "myShell.c".
 - Type the command "make"
 - And then, type "./myShell"
 - You have to press enter once to set the Profile file

## Test Plan: 

The application provided here is robust. To test its functionality, the following test cases can be used.

1. **SET 1** - Ensuring that Set 1 Functionality works properly.
Test: Performing a trial run 

Test
- For `exit()` type exit in the shell.

Result:
- The exit command will kill all the processes and exit.

Test
- For `no arguments like ls` type `ls` and press enter.

Result:
- The command uses execv and should display the list of elements.

Test
- For `commands with arguments` type `ls -l` and press enter.

Result:
- The command uses execv and should display the list of elements with the L Flag.

Test
- For `Background Processes` type `sleep 5 &` and press enter.

Result:
- The command uses waitpid and executes the process in the background.

2. **SET 2** - Ensuring that Set 2 Functionality works properly.
Test: Performing a trial run 

Test
- For `Output redirection` type `ls -lt > foo.txt` and press enter.

Result:
- The command uses freopen to write the output to a file.

Test
- For `Input redirection` type `sort < foo.txt` and press enter.

Result:
- The command uses freopen to read from a file.

Test
- For `Piping` type `ls -lt | wc` and press enter.

Result:
- The command uses pipe(), dup2() to make the pipe functionality work.

3. **SET 3** - Ensuring that Set 3 Functionality works properly.
Test: Performing a trial run 

Test
- For `Environment Variables` type `echo $myPATH` and press enter.

Result:
- The command uses execv functionality, and it reads from the profile file to save and output the environment variables.

Test
- For `Environment Variables` type `echo $myPATH` and press enter.

Result:
- The command uses execv functionality, and it reads from the profile file to save and output the environment variables.

Test
- For `Profile File`, go to the user's home directory to find all the environment variables in the file called `CIS3110_profile`.

Result:
- In the profile file, you'll find the environment variables.

Test
- For `export`, type `export myPATH=/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/bin:$myHOME/bin` and press enter.

Result:
- In the profile file, you can find that the path variable has been changed, and to test it, you can type `echo $myPATH` and enter.

Test
- For `history`, type `history`, `history -c`, or`history n` and press enter to get all the commands you have typed, to delete your history of commands, or to get the "n" number of history commands.

Result:
- In the profile file, you can find the history file named `CIS3110_history`.

Test
- For `cd`, type `cd ..` and press enter to change the current working directory.

Result:
- Shell's working directory will change.

### Citations

-Took reference from CodeVault Youtube Channel for piping.
-Took reference from Stack Overflow.
-Took reference from documentation provided by Professor Deborah Stacey

### Future Enhancements

The code is modular in nature. It has the ability, however, to be built up from even smaller components in a truly modular way utilizing more powerful tools provided by the C language. As a result, in future generations of this code, little restructuring would make it more portable.

Most of the components have been broken down to simple functions. These functions can be changed, and updated to make future enhancements. It should be pretty easy to change the code without changing the overall behavior.



**Name: Basil Yusuf**
**Course: cis3110**
**Assignment: Scheduling Algorithms**
**Date: 19th March, 2022**

## compiling
Compile using: make
Copy paste this command: `./simcpu [-d] [-v] [-r quantum] < input_file`


## Things to take care of

1. Make sure the file is of the correct format

# Operating Systems ***Memory Simulation***

**Name:** Basil Yusuf
**Course:** Operating Systems (CIS*3110)
**Date:** April 10th, 2022
**Version:** 1.0 


# Compiling Instructions: `make`

# Running Instructions: 

1. First fit: ./hole [File Name] first
1. Worst fit: ./hole [File Name] worst  
1. Best fit: ./hole [File Name] best  
1. Next fit: ./hole [File Name] next