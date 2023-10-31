///Description: A Simple Shell is a program that allows a user to use services provided *  by the operating system. It is a command line interface that takes input commands    *  from the user and provides the output to the user after it executes the input command

/// Programming language: C language

/// Input information: From the user (as commands) from the command line.

/// Compiling the program:
/// gcc minishell_part1.c -o minishell_part1
/// ./minishell_part1 
 
/// Fututer Extension or bugs:
/// This program cannot take cd commands.
/// This program does not take piped commands.


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

struct command_t {
  char * name;
  int argc;
  char * argv[MAX_ARGS];
};

char cmdLine[MAX_LINE_LEN]; //Global Variable

/// Method name: parsePath
/// Purpose: Before the program begins reding the command lines, it needs to parse the *  
/// PATH variable which is done by this method.
/// Return value: 0
/// List of all parameters: i, pathEnvVar, thePath, dirs[], 
/// Method direction: Comes from the main method

int parsePath(char * dirs[]) {
  int i = 0; ///for the for loop
  char * pathEnvVar;  /// to get the environment variable of PATH
  char * thePath;     ///to allocate memory of the environment variable of PATH

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

/// Method name: printPrompt
/// Purpose: To print the current directory and name of the shell
/// Return value: void. Prints the name of the shell
/// List of all parameters:currentDir[MAX_PATHS] 
/// Method direction: Comes from the main method

void printPrompt() {
  char currentDir[MAX_PATHS];      ///to set the current directory name 
  getcwd(currentDir, MAX_PATHS);  ///gets the current directory
  printf("\033[35mShell@Group4:\035\033[33m~%s\033[0m$ ",currentDir);

}

/// Method name: parseCommand
/// Purpose: This block of code parses the command passed from the user before it is executed. It helps to set the argc and name of the input
/// Return value: 1
/// List of all parameters: argc, clPtr,  
/// Method direction: Comes from the main method

int parseCommand(char * cLine, struct command_t * cmd) {
  ///determine command name and construct the parameter list

  int argc;      /// initializing the parameter for storing argc value of the command
  char ** clPtr; /// initializing the pointer 

  ///setting up the initialization
  clPtr = & cLine;
  argc = 0;
  cmd -> argv[argc] = (char * ) malloc(MAX_ARG_LEN);

  ///filling argv[]*
  while ((cmd -> argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
    cmd -> argv[++argc] = (char * ) malloc(MAX_ARG_LEN);
  }

  ///Setting the command name and argc
  cmd -> argc = argc - 1;
  cmd -> name = (char * ) malloc(sizeof(cmd -> argv[0]));
  strcpy(cmd -> name, cmd -> argv[0]);
  return 1;
}

/// Method name: readCommand
/// Purpose: This method reads the input provided by the user from the command line
/// Return value: command input from the user 
/// List of all parameters: command 
/// Method direction: Comes from the main method

void readCommand(char * command) {

  fgets(command, MAX_ARGS, stdin);  //getting the input of the user
  if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n') || (command[strlen(command) + 1] == '|')) { // remove trailing newline
    command[strlen(command) - 1] = '\0';
  }

}

/// Method name: lookupPath
/// Purpose: After a command is read, it needs to see if there is an executable file in one of the direcotries specified by the PATH variable
/// Return value: result or NULL
/// List of all parameters: i, pName, result, temp, dirs 
/// Method direction: Comes from the main method

char * lookupPath(char ** argv, char ** dirs) {
  int i;                       /// for the for loop
  char pName[MAX_PATH_LEN];   /// initializing the variable for path variable
  char * result;              ///initializing to store the result
  char * tmp = (char * ) malloc(MAX_PATH_LEN);   ///allocating a memory for the path variable

  if ( * argv[0] == '/') {    ///if file name is an absolute path
    result = argv[0];
    return result;
  }
  for (i = 0; i < MAX_PATHS; i++) {
    if (dirs[i] == NULL) {
      break;
    }
    strcpy(tmp, dirs[i]);    ///Copy dirs[i] to tmp.
    strcat(tmp, "/");       ///Append no more than N characters from SRC onto tmp.
    strcat(tmp, argv[0]);
    strcpy(pName, tmp);

    if (access(pName, 0) == 0) {
      /* Test for existence.  */
      result = tmp;
      return result;
    }
  }
  fprintf(stderr, "%s: command not found \n", argv[0]); // file name not found in any path variable
  return NULL;
}

/// Method name: main
/// Purpose: This method is the main method and everything starts from here
/// Return value: no return value
/// List of all parameters: argc, argv[], i, pid, status, fp, pathv,

int main(int argc, char * argv[]) {
  /* Shell initialization */
  int i; /// for the for loop
  int pid; /// pid for getting id of the process. 
  FILE * fp; ///initialization for the file
  char * pathv[MAX_PATHS];  ///initialization for the path
  struct command_t command, * cmdPtr; ///structure initialization
  
  printf(R"EOF(
      _.--._  _.--._
,-=.-":;:;:;\':;:;:;"-._
\\\:;:;:;:;:;\:;:;:;:;:;\
 \\\:;:;:;:;:;\:;:;:;:;:;\
  \\\:;:;:;:;:;\:;:;:;:;:;\
   \\\:;:;:;:;:;\:;::;:;:;:\         !!Welcome to SIMPLE SHELL!!
    \\\;:;::;:;:;\:;:;:;::;:\           Developed by Group 4
     \\\;;:;:_:--:\:_:--:_;:;\           for the purpose of Project
      \\\_.-"      :      "-._\
       \`_..--""--.;.--""--..__\
       
      Type help to see some useful commands in this shell
      Type exit to exit the shell.
      )EOF");
  printf("\n");

  parsePath(pathv); /*Get directory paths from PATH */

  while (1) {
    printPrompt();

    fp = fopen(argv[1], "r");
    readCommand(cmdLine); /*read the command line and parse it */

    if (!strcmp("", cmdLine)) { /*handling empty commands */
      continue;
    }

    if (!strcmp("exit", cmdLine)) { /*exiting the shell */

      break;

    }
    if (!strcmp("help", cmdLine)) { /* help for the shell */
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

    parseCommand(cmdLine, & command);
    command.name = lookupPath(command.argv, pathv);

    if ((pid = fork()) == 0) { /* creating the child process */
      
      execv(command.name, command.argv); /* executing the command */
      exit(0); /*termination of the child process */

    } else {
      wait(NULL); /*waits for the child process to terminate */
    }
    if (command.name != NULL) { /*frees the command argc values */
      free(command.name);
    }
  }

  printf("Terminiting Shell 3 seconds\n");
  sleep(3);
  exit(0); /*exiting the loop*/
}
