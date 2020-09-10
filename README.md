# ASH - Aakash's Shell
## What is it?
ASH is a shell coded in C that emulates the behavior of BASH. Some functions were coded from scratch, while others are run using the exec() family of functions.
## How to use
Download all the files and put them in a directory. Use the ```cd``` command to navigate to the directory, and then run the ```make``` command. This will create an executable ```ash``` which can be run using the command ```./ash```. If the executable does not have execute permissions, please run ```chmod +x ash``` to provide execute permissions. ASH should start in the same terminal.
## Features
Semicolon seperated commands provide the ability to run multiple commands concurrently.  
Implementations of ls, cd, echo etc.  
A new pinfo function that allows you to view basic information of any process.
Background process functionality allows you to run any command in the background, and still use your shell.  
A history function to keep a track of entered commands.  
## File Structure
### include
#### Shell.h
Contains all the header files that are used for the functions in the shell. Also has some definitions on sizes, and for structures used in the program.
#### Functions.h
Contains definitions of all functions.
#### Variables.h
Contains definitions of all global variables used in the shell. Global variables are used instead of local variables to simplify function calls.
#### history
Contains 20 previous commands. Used for the history shell command (read and write).
### src
#### Main.c
Initializes the signal handler, display, home directory etc. Runs infinitely calling the ash_main function. Before exiting kills all background processes spawned.  
#### ash_main.c
Displays the prompt and takes input from the user using the take_inp function. Cleans the input, and parses it based on semicolon. Calls the appropriate function based on the first word of the input.  
#### General_Functions.c 
Has all the functions that serve as utility functions. The shell functions use all of these (setting directories, displaying, clearing screen etc).  
#### ash_echo.c
Basic implementation of echo, lacks quotation and escape sequence functionality.
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
Contains implementation to read and write from the history file. 
#### ash_watch.c
Contains implementation for the nightswatch command. The function keeps a track of number of keyboard interrupts per CPU or the PID of the most recently created process.
### extra
#### Makefile
Contains the series of instructions required to compile the code, and create the executable. Run the program using the make command.
## Command usage
#### echo
Syntax : ```echo <string>```
	Echo simply dumps the string provided as an argument onto stdout. It does not support escape sequences or quotation marks yet.
#### cd
Syntax : ```cd <dirname>```
	Cd changes the current directory to the one specified by the path (relative or absolute). also supports ```. .. ~```. Cd with no arguments is invalid (contrary to bash) as it is entered accidentally many times.
#### pwd
Syntax : ```pwd```
	Pwd displays the current directory (absolute path). Ignores all extra parameters passed.
#### ls
Syntax : ```ls -<flags> <dirlist>```
	Ls displays all the files/directories present in the directories specified. Ls supports ```. .. ~```, and the flags ```-l -a```. The flags can be concatenated in whichever way and will still be valid. Ls will assume that no directory specified implies the current directory.
#### pinfo
Syntax : ```pinfo <PID>```
	Pinfo displays information about the process specified. Information includes PID, status, memory and executable path in relative style. Pinfo will assume that no PID specified means the ash process itself. Since pinfo reads from /proc/pid/stat, it is designed to run on linux kernels only.
#### history
Syntax : ```history <num>```
	History displays the previous num commands (including the history command itself). Default value for num is 10, and any value >20 will be rounded down to 20. 
#### nightswatch
Syntax : ```nightswatch -n <delay> <interrupt/newborn>```
	Interrupt - Displays number of keyboard interrupts per CPU every delay seconds.
	Newborn - Displays the PID of the last created process every delay seconds.
	Minimum delay is 1 second, and default delay is 5 seconds.
#### list
Syntax : ```list```
	List displays a list of PID and names of the current background processes spawned by ash.
## Assumptions
Ash is designed to be used for normal day to day needs and hence certain reasonable assumptions were made on the length of directory, file and command names.  
Since this shell has been created in an attempt to learn, error handling is not very robust and ash is not intended to be stress-tested.  
Only 1024 background processes can be created in every ash instance.  
History size is limited to 20, but can be increased by tweaking the value in the Shell.h header file.  
Pinfo reads from the /proc/pid/stat file and hence its usage is restricted to linux kernels.  
Interrupt handling has not been implemented yet.  
## Future updates
Support for piping, redirection, and proper interrupt handling.  
Color schemes, and output formatting for visual aesthetics.  
Manual entries for new functions that come with the shell.  