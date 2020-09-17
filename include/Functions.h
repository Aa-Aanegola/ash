/*
	A header file with all the declarations of all functions used in the shell.
*/
#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef FUNC_H
#define FUNC_H

// Used to clear the display
void clear_disp();

// Takes the string passed as argument and dumps it onto stdout
void disp(char *str);

// Displays a newline
void newl();

// Reads user input into buffer_command
void take_inp();

// Updates the display name in case the directory has been changed
void update_disp();

// Initializes the display with the username and hostname
void initialize_disp();

// Removes redundant whitespace from the string passed to it
void clean_string(char *str);

// Extracts the first token (command word) from read_in
void get_command();

// COnverts the shorthands for directories into the full path (. to the current working directory etc.)
void check_dir();

// Reads the home directory of the shell into home_dir
void get_home();

// The function that is called from main and does the parsing
void ash_main();

// Implementation of the echo command
void ash_echo();

// Implementation of the pwd command
void ash_pwd();

// Implementation of the cd command
void ash_cd();

// Implementation of the ls command
void ash_ls();

// Reads the file permissions into permissions
void get_perm(char *file);

// Implementation of the pinfo command
void ash_pinfo();

// Calls execvp to execute all other commands, also supports background process creation
void ash_general();

// Implementation of the nightswatch command
void ash_watch();

// Generic signal hanfler template
handler* install_signal(int signum, handler* handler);

// Specific child handler that takes care of SIGCHLD signal
void child_handler(int sig, siginfo_t* info, void* vp);

// Handles Ctrl+Z
void z_handler(int sig, siginfo_t* info, void* vp);

// Handles Ctrl+C
void c_handler(int sig, siginfo_t* info, void* vp);


// Reads from history, implementation of history command
void ash_history_read();

// Writes every command to the history
void ash_history_write();

// Initializes the child pool to NULL
void init_child_proc();

// Pushed a child process (if background) into the child pool
void push_child(pid_t pid);

// Sorts all children
void sort_child();

// Lists all background processes
void ash_jobs();

// Sends signal specified to job specified
void ash_kjob();

// Kills all children before exiting the program
void child_kill();

// Checks if the user inputted < > or >>
void ash_redir();

// Maps the commands to functions
void exec_builtin();

// Sets environment variables
void env_set();

// Unsets environment variables
void env_unset();

// Checks for pipes
void ash_pipe();

// Sends a process to the background
void ash_bg();

// Brings a process to the foreground
void ash_fg();
#endif