/*
A list of all the header files that all my functions use and my structure for process data
*/
#ifndef SHELL_H
#define SHELL_H

#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<ctype.h>
#include<dirent.h>
#include<grp.h>
#include<time.h>
#include<signal.h>
#include<stddef.h>
#include<sys/sysinfo.h>

// For signal processing
typedef void handler(int, siginfo_t *, void *);

#endif