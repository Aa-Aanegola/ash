#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Initializes all the child process structures in the pool to NULL
void init_child_proc()
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		proc_array[i].pid = -1;
		proc_array[i].name[0] = '\0';
		proc_array[i].pos = INF;
	}
}

// Given the PID, adds a new child process to the pool in the first free location
void push_child(pid_t pid)
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid == -1)
		{
			proc_array[i].pid = pid;
			strcpy(proc_array[i].name, command_word);
			proc_array[i].pos = num_children++;
			return;
		}
	}	
	
	// In case the pool is full 
	write(2, "ash: general: Process insertion failed", strlen("ash: general: Process insertion failed"));
	newl();
}

void sort_child()
{
	for(int i = 0; i<POOL_SIZE; i++)
		if(proc_array[i].pid == -1)
			proc_array[i].pos = INF;


	for(int i = 0; i<POOL_SIZE; i++)
	{
		for(int j = 0; j<POOL_SIZE-1-i; j++)
		{
			if(proc_array[j].pos > proc_array[j+1].pos)
			{
				child_proc temp;
				temp.pid = proc_array[j].pid;
				strcpy(temp.name, proc_array[j].name);
				temp.pos = proc_array[j].pos;

				proc_array[j].pid = proc_array[j+1].pid;
				strcpy(proc_array[j].name, proc_array[j+1].name);
				proc_array[j].pos = proc_array[j+1].pos;


				proc_array[j+1].pid = temp.pid;
				strcpy(proc_array[j+1].name, temp.name);
				proc_array[j+1].pos = temp.pos;
			}
		}
	}

	for(int i = 0; i<POOL_SIZE; i++)
		proc_array[i].pos = i;
}

// This is used for the list functionality, displays the info of all background processes currently in the pool
void ash_jobs()
{
	char *buffer = (char*)malloc(MAX_COMM*sizeof(char));
	char *file_name = (char*)malloc(MIN_COMM*sizeof(char));

	char stat;

	sort_child();

	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid != -1)
		{
			sprintf(file_name, "/proc/%d/stat", proc_array[i].pid);
			FILE *fp = fopen(file_name, "r");

			if(fp == NULL)
				proc_array[i].pid = -1;
		}
	}

	sort_child();

	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid != -1)
		{
			sprintf(file_name, "/proc/%d/stat", proc_array[i].pid);
			FILE *fp = fopen(file_name, "r");

			if(fp != NULL)
			{
				pid_t pid;
				fscanf(fp, "%d", &pid);
				fgetc(fp);
				stat = fgetc(fp);
				while(stat != ')')
					stat = fgetc(fp);
				while(stat  == ')')
					stat = fgetc(fp);
				fscanf(fp, "%c", &stat);

				if(stat == 'T' || stat == 't')
					sprintf(buffer, "[%d] Stopped", proc_array[i].pos+1);
				else
					sprintf(buffer, "[%d] Running", proc_array[i].pos+1);
				sprintf(buffer, "%s %s [%d]\n", buffer, proc_array[i].name, proc_array[i].pid);
				disp(buffer);
			}
		}
	}
}

// Iterates through the process pool and issues a SIGKILL command to each of them. Prevents SIGHUP signal sending
void child_kill()
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid != -1)
			kill(proc_array[i].pid, SIGKILL);
	}
}
