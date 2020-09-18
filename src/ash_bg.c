#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Changes the state of the specified background process to running
void ash_bg()
{
	// Variables for parsing
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	int count = 0;
	
	// Count number of parameters, and handle errors
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		count++;
		token = strtok(NULL, " ");
	}

	if(count != 2)
	{
		write(2, "ash: bg: Only two arguments must be passed", strlen("ash: bg: Only two arguments must be passed"));
		newlerr();
		suc_flag = 1;
		return;
	}

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");

	int job_no = atoi(token) - 1;

	// Iterate over job pool and find desired job number
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos == job_no)
		{
			if(proc_array[i].pid == -1)
				break;
			
			// If found then send process the SIGCONT signal
			if(kill(proc_array[i].pid, SIGCONT) < 0)
			{
				write(2, "ash: Couldn't send signal to process", strlen("ash: Couldn't send signal to process"));
				newlerr();
				suc_flag = 1;
			}
			return;
		}
	}

	// If the job number is invalid, display error message
	write(2, "ash: bg: Invalid job number", strlen("ash: bg: Invalid job number"));
	newlerr();
	suc_flag = 1;
}