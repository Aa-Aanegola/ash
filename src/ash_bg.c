#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_bg()
{
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
		write(2, "ash: bg: Only two arguments must be passed", strlen("ash: bg: Only two arguments must be passed"));
		newl();
		return;
	}

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");

	int job_no = atoi(token) - 1;

	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pos == job_no)
		{
			kill(proc_array[i].pid, SIGCONT);
			return;
		}
	}

	write(2, "ash: bg: Invalid job number", strlen("ash: bg: Invalid job number"));
	newl();
}