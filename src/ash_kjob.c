#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_kjob()
{
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	int jno, signo;

	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");

	if(token == NULL)
	{
		write(2, "ash: kjob: No job number specified", strlen("ash: kjob: No job number specified"));
		newl();
		return;
	}
	jno = atoi(token);
	if(jno == 0)
	{
		write(2, "ash: kjob: Job number doesn't exist", strlen("ash: kjob: Job number doesn't exist"));
		newl();
		return;
	}

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		write(2, "ash: kjob: No signal number specified", strlen("ash: kjob: No job number specified"));
		newl();
		return;
	}
	signo = atoi(token);

	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos+1 == jno)
		{
			if(kill(proc_array[i].pid, signo) < 0)
			{
				write(2, "ash: kjob: signal could not be sent", strlen("ash: kjob: signal could not be sent"));
				newl();
			}
			return;
		}
	}

	write(2, "ash: kjob: Job number doesn't exist", strlen("ash: kjob: Job number doesn't exist"));
}