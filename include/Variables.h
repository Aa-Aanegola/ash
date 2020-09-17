/*
	A list of all the global variables used in my functions
*/
#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#define VARIABLE_H

// Entire semicolon seperated list of commands is stored in buffer_command
char buffer_command[5*MAX_COMM];

// Individual commands are stored in read_in
char read_in[MAX_COMM];

// Host name is stored in hname
char hname[MIN_COMM];

// Display name (username@hostname) is stored in display_name
char display_name[MAX_COMM];

// First token of each command is stored in command_word
char command_word[MIN_COMM];

// The directory from which the shell is invoked, short for home directory
char home_dir[MAX_COMM];

// The directory that is currently open, short for current directory
char cur_dir[MAX_COMM];

// Used for ls -l, stores permissions and file type
char permissions[11];

// Used for changing directories and ls, short for specified directory
char spec_dir[MAX_COMM];

// stores the target destination, used in tandem with spec_dir
char target[MAX_COMM];

// Universal flag to signify exit from program
int uflag;

// Pool for all child processes
child_proc proc_array[POOL_SIZE];

// Number of children currently in the pool
int num_children;

// Holds the stdin code
int master_in;

// Holds the stdout code
int master_out;

// Holds the pid of the root (parent)
pid_t master_pid;

// Holds the pid of the foreground process
child_proc fore_proc;

#endif