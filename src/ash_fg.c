#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Brings the process specified by its job number to the foreground
void ash_fg()
{
	// Sort all the background processes
	sort_child();

	pid_t par_pid = getpid();


	// Variables used for tokenization
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	int count = 0;
		
	// Parses argument and checks for invalid syntax
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		count++;
		token = strtok(NULL, " ");
	}

	if(count != 2)
	{
		write(2, "ash: fg: Only two arguments must be passed", strlen("ash: bg: Only two arguments must be passed"));
		newlerr();
		suc_flag = 1;
		return;
	}

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
	int job_no = atoi(token)-1;

	// Searches for job specified
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos == job_no)
		{
			if(proc_array[i].pid == -1)
				break;

			// If the job number is valid, ignore the SIGTTOU and SIGTTIN and set it to foreground with respect to stdin
			int status;
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
			tcsetpgrp(STDIN_FILENO, proc_array[i].pid);
			
			// Try sending SIGCONT signal, and handle errors
			if(kill(proc_array[i].pid, SIGCONT) < 0)
			{
				write(2, "ash: fg: Failed to bring process to foreground", strlen("ash: fg: Failed to bring process to foreground"));
				newlerr();
				suc_flag = 1;
				return;
			}

			// To prime for Ctrl+Z
			fore_proc.pid = proc_array[i].pid;
			strcpy(fore_proc.name, proc_array[i].name);
				
			// Wait till the process exits
			waitpid(proc_array[i].pid, &status, WUNTRACED);
			fore_proc.pid = -1;

			// Set the signal handlers back to default, and set the shell to foreground
			tcsetpgrp(STDIN_FILENO, par_pid);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);

			// If the process was terminated, then remove it from jobs list
			if(WIFEXITED(status))
			{
				proc_array[i].pid = -1;
				proc_array[i].pos = INF;
			}

			return;
		}
	}

	// In cast the arguments passed were invalid
	write(2, "ash: fg: Job number invalid", strlen("ash: fg: Job number invalid"));
	newlerr();
	suc_flag = 1;
}