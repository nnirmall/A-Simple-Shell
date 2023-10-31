# A-Simple-Shell

##  Description
Description
A shell is a program that allows a user to use services provided by the operating
system. It is a command line interface that takes input commands from the user and
provides the output to the user after it executes the input commands.
## Prerequisites
- C or C++ compiler
- UNIX Operating System
## Compiling & Running
There are two shells and the implementation for each is different. Part 1 Shell uses
basic execv whereas Part 2 Shell uses thread and system to execute commands. If you
want to use the first shell use Part 1 and for another use Part 2.
Part 1 and Part 2 are just names for two two shells not the part of a minishell.
### Part 1:
```bash
$gcc minishell_part1.c -o minishell_part1
$./minishell_part1
```
### Part 2:
```bash
$gcc minishell_part2 minishell_part2.c -lpthread
$./minishell_part2
```
## Features:
- Runs basic linux commands like other shell
- Cool user interface
- User can exit the shell by hitting exit command 
## Fututer Extension or bugs:
- Part 1 minishell program cannot take cd commands.
- Part 1 minishell program does not take piped commands.
