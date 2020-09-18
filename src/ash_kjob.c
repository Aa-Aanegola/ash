#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif
	
// Send the job specified the signal specified
void ash_kjob()
{
	// Variables for tokenization
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	// The arguments sent 
	int jno, signo;

	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");

	// Checking validity of the kjob command
	if(token == NULL)
	{
		write(2, "ash: kjob: No job number specified", strlen("ash: kjob: No job number specified"));
		newlerr();
		suc_flag = 1;
		return;
	}
	jno = atoi(token);
	if(jno == 0)
	{
		write(2, "ash: kjob: Job number doesn't exist", strlen("ash: kjob: Job number doesn't exist"));
		newlerr();
		suc_flag = 1;
		return;
	}

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		write(2, "ash: kjob: No signal number specified", strlen("ash: kjob: No job number specified"));
		newlerr();
		suc_flag = 1;
		return;
	}
	signo = atoi(token);

	// Iterating over all the background processes, and sending the one with the specified job number the signal.
	// Loop is required because of random entry of processes into the pool
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos+1 == jno)
		{
			if(proc_array[i].pid == -1)
				break;
			
			if(kill(proc_array[i].pid, signo) < 0)
			{
				write(2, "ash: kjob: signal could not be sent", strlen("ash: kjob: signal could not be sent"));
				newlerr();
				suc_flag = 1;
			}
			return;
		}
	}

	// In case an invalid job number is specified
	write(2, "ash: kjob: Job number doesn't exist", strlen("ash: kjob: Job number doesn't exist"));
	newlerr();
	suc_flag = 1;
}