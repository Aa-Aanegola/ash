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

handler* install_signal(int signum, handler* handler);

void child_handler(int sig, siginfo_t* info, void* vp);

void ash_history_read();

void ash_history_write();

#endif