# ASH - Aakash's Shell
## What is it?
ASH is a shell coded in C that emulates the behavior of BASH, but adds the functionality of semicolon seperated commands. Some functions were coded from scratch, while others are run using the exec() family of functions.
## How to use
After downloading all the files, and putting them into the folder of your choice, open a terminal window and run the command make. This will create an executable that you can use by running the command ./ash in your terminal window. This will flash a prompt on your screen after which you are free to enter commands of your choice.
## Features
Semicolon seperated commands provide the ability to run multiple commands concurrently.  
Implementations of ls, cd, echo etc.  
A new pinfo function that allows you to view basic information of any process.
Background process functionality allows you to run any command in the background, and still use your shell.  
A history function to keep a track of entered commands.  
## List of commands
echo : equivalent to BASH echo, lacks support for quotes and backslashes.  
pwd : displays the present working directory  
cd : changes current directory to the directory specified.  
ls : lists the files present in the directory specified, has support for -a,  -l flags and multiple directories.  
pinfo : prints the process information of the process id specified.  
history : prints the previous 10 entered commands (including the history command), can also specify number of commands to be displayed.  
nightswatch : tracks number of keyboard interrupts from /proc/interrupts.  
## Future updates
Support for piping, redirection, and proper exit handling.  
Color schemes, and output formatting for visual aesthetics.  
Manual entries for new functions that come with the shell.  