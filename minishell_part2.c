///Description: A Simple Shell is a program that allows a user to use services provided *  by the operating system. It is a command line interface that takes input commands    *  from the user and provides the output to the user after it executes the input command

/// Programming language: C language

/// Input information: From the user (as commands) from the command line.

/// Compiling the program:
/// gcc minishell_part2 minishell_part2.c -lpthread
/// ./minishell_part1\2
 
/// Fututer Extension or bugs:
/// system function is invoked which is quite dangerous

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_PATHS 64
#define WHITESPACE " .,\t\n"
#define MAX_LINE_LEN 80
#define MAX_PATH_LEN 96
#ifndef NULL
#define NULL
#endif

#include <sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include <pthread.h>

//Initializing the structure
struct command_t {
  char * name;
  int argc;
  char * argv[MAX_ARGS];
};

char cmdLine[MAX_LINE_LEN]; ///Global variable

/// Method name: printPrompt
/// Purpose: To print the current directory and name of the shell
/// Return value: void. Prints the name of the shell
/// List of all parameters:currentDir[MAX_PATHS] 
/// Method direction: Comes from the main method

void printPrompt() {
  char currentDir[MAX_PATHS];
  getcwd(currentDir, MAX_PATHS);
  printf("\033[35mShell@Group4:\035\033[33m~ %s\033[0m$ ", currentDir);

}

/// Method name: readCommand
/// Purpose: This method reads the input provided by the user from the command line
/// Return value: command input from the user 
/// List of all parameters: command 
/// Method direction: Comes from the main method

void readCommand(char * command) {
  fgets(command, MAX_ARGS, stdin);
  if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) { // remove trailing newline
    command[strlen(command) - 1] = '\0';
  }

}

/// Method name: parsePath
/// Purpose: Before the program begins reding the command lines, it needs to parse the *  
/// PATH variable which is done by this method.
/// Return value: 0
/// List of all parameters: i, pathEnvVar, thePath, dirs[], 
/// Method direction: Comes from the main method

int parsePath(char * dirs[]) {
  int i = 0;
  char * pathEnvVar;
  char * thePath;

  for (i = 0; i < MAX_ARGS; i++) {
    dirs[i] = NULL;
  }
  pathEnvVar = (char * ) getenv("PATH");
  thePath = (char * ) malloc(strlen(pathEnvVar) + 1);
  strcpy(thePath, pathEnvVar);

  for (i = 0; i < MAX_PATHS; i++) {
    dirs[i] = strsep( & thePath, ":");
    if (dirs[i] == NULL)
      break;
  }

  return 0;
}
/// Method name: thread1
/// Purpose: to execute the command through thread 
/// Return value: 0
/// List of all parameters: command 
/// Method direction: Comes from the main method

void * thread1(void * command) { //thread t1 Routine
  system(command);
  pthread_exit(0);
}

int main(int argc, char * argv[]) {
  //Shell initialization
  char * pathv[MAX_PATHS];
  struct command_t command, * cmdPtr;
  int status;
  pthread_t thread; //Initializing thread

  printf(R"EOF(
       .---.
      /     \
      \.@-@./
      /`\_/`\
     //  _  \\          !!Welcome to SIMPLE SHELL!!
    | \     )|_             Developed by Group 4
   /`\_`>  <_/ \              for the purpose of Project
   \__/'---'\__/
       
      Type help to see some useful commands in this shell
      Type exit to exit the shell.
      )EOF");
  
  printf("\n");

  parsePath(pathv); //Get directory paths from PATH 

  while (1) {
    printPrompt();

    readCommand(cmdLine); //read the command line

    if (!strcmp("help", cmdLine)) { //help section of the shell
      printf(R"EOF(
      Some builtin commands are:
      -> help : to see some useful commands in this shell
      -> exit : to exit the shell.


      Some basic linux commands are:
      -> pwd  : to show the current directory
      -> ls   : to show the contents of the directory
      -> ps   : to show current processes
      -> ps -ef: to show detailed processes
      -> date : to show current date
      -> mkdir DIRECTORyNAME: to make a directory
      -> and there are many more.......

      this shell does not execute cd command
      )EOF");
      printf("\n");
      continue;
    }
    if (!strcmp("exit", cmdLine)) { //exiting the shell

      break;

    }

    pthread_create( & thread, NULL, thread1, & cmdLine); // Creating thread
    pthread_join(thread, (void ** ) & status);   //Joining thread
  }
  
  printf("Terminiting Shell 3 seconds\n");  
  sleep(3);
  exit(0);

}
