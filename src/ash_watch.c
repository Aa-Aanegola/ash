#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_watch()
{
	// Default wait time
	int pause = 5;
	// Interrupt/Newborn/Invalid - default is invalid
	int flag = -1;
	
	// String variables that are used for parsing the command
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	token = strtok(dup_in, " ");

	// Parsing the input to get the necessary flags using strtok
	while(token != NULL)
	{
		token = strtok(0, " ");
		if(token == NULL)
			break;
		if(token[0] == '-')
		{
			if(token[1] != 'n')
				break;
			token = strtok(0, " ");
			if(token == NULL)
				break;
			sscanf(token, "%d", &pause);
		}
		else
		{
			if(!strcmp(token, "interrupt"))
				flag = 0;
			else if(!strcmp(token, "newborn"))
				flag = 1;
			else
				break;
		}
	}

	// Default value for rest is 1 second
	if(pause <= 0)
		pause = 1;

	// Error message
	if(flag == -1)
	{
		write(2, "ash: nightswatch: Invalid option", strlen("ash: nightswatch: Invalid option"));
		newl();
		return;
	}

	// Tracking interrupts
	if(flag == 0)
	{
		// Using a child process to display the interrupt count, and parent to track for exit command
		int pid = fork();

		if(pid < 0)
		{
			write(2, "ash: nightswatch: Process failed", strlen("ash: nightswatch: Process failed"));
			newl();
			return;
		}

		if(pid == 0)
		{
			FILE *fp = fopen("/proc/interrupts", "r");
			char *buffer = (char*)malloc(MIN_COMM*sizeof(char));
			int num_proc = sysconf(_SC_NPROCESSORS_ONLN);
			int interr[16];

			buffer[0] = '\0';
			for(int i = 0; i<num_proc; i++)
				sprintf(buffer, "%sCPU%d\t", buffer, i);
			
			disp(buffer);
			newl();

			fclose(fp);

			while(1)
			{
				fp = fopen("/proc/interrupts", "r");
				fscanf(fp, "%[^ ]", buffer);
				while(strcmp(buffer, "1:"))
				{
					fscanf(fp, "%[^ ]", buffer);
					fgetc(fp);
				}
				buffer[0] = '\0';
				for(int i = 0; i<num_proc; i++)
					fscanf(fp, "%d", &(interr[i]));
				for(int i = 0; i<num_proc; i++)
					sprintf(buffer, "%s%d\t", buffer, interr[i]);
				disp(buffer);
				newl();

				fclose(fp);
				sleep(pause);
			}
		}

		else
		{
			char ch;
			while(1)
			{
				ch = getchar();
				if(ch == 'q')
				{
					kill(pid, SIGKILL);
					return;
				}
			}
		}
	}

	// Tracking new process creation
	else if(flag == 1)
	{
		// Using child process to track /proc/loadavg and parent to check for exit command
		int pid = fork();

		if(pid < 0)
		{
			write(2, "ash: nightswatch: Process failed", strlen("ash: nightswatch: Process failed"));
			newl();
			return;
		}

		if(pid == 0)
		{
			while(1)
			{
				FILE *fp = fopen("/proc/loadavg", "r");
				char *buffer = (char*)malloc(MIN_COMM*sizeof(char));
				
				int c = 0;
				char temp;
				while(c < 4)
				{	
					temp = fgetc(fp);
					if(temp == ' ')
						c++;
				}

				fgets(buffer, MIN_COMM, fp);
				disp(buffer);
				sleep(pause);
			}
		}

		else
		{
			char ch;
			while(1)
			{
				ch = getchar();
				if(ch == 'q')
				{
					kill(pid, SIGKILL);
					return;
				}
			}
		}	
	}
}