/*
	A list of all the global variables used in my functions
*/
#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#define VARIABLE_H

// Entire semicolon seperated list of commands is stored in buffer_command
char buffer_command[5000];

// Individual commands are stored in read_in
char read_in[1000];

// Host name is stored in hname
char hname[256];

// Display name (username@hostname) is stored in display_name
char display_name[1000];

// First token of each command is stored in command_word
char command_word[200];

// The directory from which the shell is invoked, short for home directory
char home_dir[1000];

// The directory that is currently open, short for current directory
char cur_dir[1000];

// Used for ls -l, stores permissions and file type
char permissions[11];

// Used for changing directories and ls, short for specified directory
char spec_dir[1000];

// stores the target destination, used in tandem with spec_dir
char target[1000];

// Universal flag to signify exit from program
int uflag;

#endif