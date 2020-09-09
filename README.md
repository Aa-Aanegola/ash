# ASH - Aakash's Shell
## What is it?
ASH is a shell coded in C that emulates the behavior of BASH. Some functions were coded from scratch, while others are run using the exec() family of functions.
## How to use
After downloading all the files, and putting them into the folder of your choice, open a terminal window and run the command make from the folder. This will create an executable that you can use by running the command ./ash in your terminal window. Running the command will start the shell in the same terminal window.
## Features
Semicolon seperated commands provide the ability to run multiple commands concurrently.  
Implementations of ls, cd, echo etc.  
A new pinfo function that allows you to view basic information of any process.
Background process functionality allows you to run any command in the background, and still use your shell.  
A history function to keep a track of entered commands.  
## File Structure
### Header files
#### Shell.h
Contains all the header files that are used for the functions in the shell. Also has some definitions on sizes, and for structures used in the program.
#### Functions.h
Contains definitions of all functions. Enables modularity.
#### Variables.h
Contains definitions of all global variables used in the shell. Global variables are used instead of local variables to simplify function calls.
### C program files
#### Main.c
Initializes the signal handler, display, home directory etc. Runs infinitely calling the ash_main function. Before exiting kills all background processes spawned.  
#### ash_main.c
Displays the prompt and takes input from the user using the take_inp function. Cleans the input, and parses it based on semicolon. Calls the appropriate function based on the first word of the input.  
#### General_Functions.c 
Has all the functions that serve as utility functions. The shell functions use all of these (setting directories, displaying, clearing screen etc).  
#### ash_echo.c
Basic implementation of echo, lacks quote and \ functionality.
#### ash_cd.c
Implementation of the BASH cd command. Changes directory to the specified directory.
#### ash_pwd.c
Displays the current working directory. Ignores all other strings after command word pwd.
#### ash_ls.c
Implementation of the ls command. Has support for the -a and -l flags.
#### ash_pinfo.c
Pinfo is a command added to ash. It displays the process information of the process specified(by pid) or of the current process if a pid is not specified.
#### ash_general.c
In case ash is requested to run a command through exec, this function is called. Forks a child process to run the desired command. Contains support for background processes using '&'. Use the command list to list all the current background processes.
#### ash_history.c
Contains implementation to read and write from the history file. the history command is run by entering history, or history <num> which displays <num> previous commands.
#### ash_watch.c
Contains implementation for the nightswatch command, run using nightswatch -n <num> interrupt. Newborn functionality is absent (tracking new process creation). The function keeps a track of number of keyboard interrupts per CPU.
### Extra
#### Makefile
Contains the series of instructions required to compile the code, and create the executable. Run the program using the make command.
#### history
Contains previous 20 commands (including current). Used in the history command.
## Future updates
Support for piping, redirection, and proper exit handling.  
Color schemes, and output formatting for visual aesthetics.  
Manual entries for new functions that come with the shell.  