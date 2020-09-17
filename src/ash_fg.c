#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_fg()
{
	sort_child();

	pid_t par_pid = getpid();

	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	int count = 0;
		
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		count++;
		token = strtok(NULL, " ");
	}

	if(count != 2)
	{
		write(2, "ash: fg: Only two arguments must be passed", strlen("ash: bg: Only two arguments must be passed"));
		newl();
		return;
	}

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
	int job_no = atoi(token)-1;

	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos == job_no)
		{
			int status;
			signal(SIGTTOU, SIG_IGN);
			signal(SIGTTIN, SIG_IGN);
			tcsetpgrp(STDIN_FILENO, proc_array[i].pid);
			
			if(kill(proc_array[i].pid, SIGCONT) < 0)
			{
				write(2, "ash: fg: Failed to bring process to foreground", strlen("ash: fg: Failed to bring process to foreground"));
				newl();
				return;
			}

			fore_proc.pid = proc_array[i].pid;
			strcpy(fore_proc.name, proc_array[i].name);
			
			waitpid(proc_array[i].pid, &status, WUNTRACED);
			fore_proc.pid = -1;

			tcsetpgrp(STDIN_FILENO, par_pid);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);

			if(WIFEXITED(status))
			{
				proc_array[i].pid = -1;
				proc_array[i].pos = INF;
			}

			return;
		}
	}

	write(2, "ash: fg: Job number invalid", strlen("ash: fg: Job number invalid"));
	newl();
}