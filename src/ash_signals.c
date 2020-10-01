#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Declaration of the sigaction handler, copied from an online resource
handler* install_signal(int signum, handler* handler)
{
	struct sigaction new_action, old_action;
	
	memset(&new_action, 0, sizeof(struct sigaction));
	new_action.sa_sigaction = handler;
	sigemptyset(&new_action.sa_mask);
	
	new_action.sa_flags = SA_RESTART|SA_SIGINFO;

	if (sigaction(signum, &new_action, &old_action) < 0)
	{
		write(2, "ash: signal encountered an error", strlen("ash: signal encountered an error"));                                                         
		newlerr();
	}
    return (old_action.sa_sigaction);                                           
}


// The function that is called whenever a SIGCHLD signal is received. This was done to ensure that child exit handling is asynchronous
void child_handler(int sig, siginfo_t* info, void* vp)
{
	int status;
	int pid;
	char buffer[MAX_COMM];

	int term = 0;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		for(int i = 0; i<POOL_SIZE; i++)
		{
			if(proc_array[i].pid == pid)
			{
				term = 1;
				sprintf(buffer, "\n%s with pid %d exited %s", proc_array[i].name, pid, WIFEXITED(status) == 0 ? "abnormally" : "normally");
				write(1, buffer, strlen(buffer));
				proc_array[i].pid = -1;
				proc_array[i].pos = INF;
				num_children--;
				break;
			}
		}
	}
	if(term)
	{
		newl();
		disp(display_name);
	}
}

// Handles when you press ctrl+c - discovered that return works well because all execvp functions are reset to default signal handlers
void c_handler(int sig, siginfo_t* info, void* vp)
{
	/*pid_t pid = getpid();
	if(pid != master_pid)
		return;
	if(fore_proc.pid == -1)
		return;

	if(kill(fore_proc.pid, SIGINT) < 0)
	{
		write(2, "ash : Could not send SIGINT to process", strlen("ash : Could not send SIGINT to process"));
		newl();
		suc_flag = 1;
		return;
	}

	for(int i = 0; i<POOL_SIZE; i++)
		if(fore_proc.pid == proc_array[i].pid)
			proc_array[i].pid = -1;

	fore_proc.pid = -1;

	suc_flag = 1;*/
}

// Handles Ctrl+Z
void z_handler(int sig, siginfo_t* info, void* vp)
{
	// Checks that Ctrl+Z was passed in terminal, and checks if a foreground process exists
	pid_t pid = getpid();
	if(pid != master_pid)
		return;
	if(fore_proc.pid == -1)
		return;

	// Checks if we can move the process to background 
	if(num_children == POOL_SIZE)
	{
		write(2, "ash: Too many background processes", strlen("ash: Too many background processes"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// If we can simplt send a SIGTSTP signal to the foreground process
	setpgid(fore_proc.pid, 0);
	if(kill(fore_proc.pid, SIGTSTP) < 0)
	{
		write(2, "ash: Could not send SIGTSTP to process", strlen("ash : Could not send SIGTSTP to process"));
		newlerr();
		suc_flag = 1;
		return;	
	}

	// Reset the input stream, and make sure that the SIGTTIN and SIGTTOU signals are set back to their default handlers
	tcsetpgrp(STDIN_FILENO, master_pid);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);

	// Put the foreground process into background
	strcpy(command_word, fore_proc.name);
	push_child(fore_proc.pid);
}