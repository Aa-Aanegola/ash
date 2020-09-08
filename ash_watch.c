#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_watch()
{
	int pause = 5;
	int flag = -1;

	
	char *token;
	char *dup_in = (char*)malloc(1000*sizeof(char));
	strcpy(dup_in, read_in);

	token = strtok(dup_in, " ");

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

	if(pause == 0)
		pause = 1;

	if(flag == -1)
	{
		write(1, "ash: nightswatch: Invalid option", strlen("ash: nightswatch: Invalid option"));
		newl();
		return;
	}

	if(flag == 0)
	{
		int pid = fork();

		if(pid < 0)
		{
			write(1, "ash: nightswatch: Process failed", strlen("ash: nightswatch: Process failed"));
			newl();
			return;
		}

		if(pid == 0)
		{
			FILE *fp = fopen("/proc/interrupts", "r");
			char *buffer = (char*)malloc(200*sizeof(char));
			fscanf(fp, "%[^ ]", buffer);
			while(strcmp(buffer, "1:"))
			{
				fscanf(fp, "%[^ ]", buffer);
				fgetc(fp);
			}
			int pos = ftell(fp);
			int num_proc = sysconf(_SC_NPROCESSORS_ONLN);
			int interr[16];

			buffer[0] = '\0';
			for(int i = 0; i<num_proc; i++)
				sprintf(buffer, "%sCPU%d\t", buffer, i);
			
			disp(buffer);
			newl();

			while(1)
			{
				fseek(fp, 0, pos);
				buffer[0] = '\0';
				for(int i = 0; i<num_proc; i++)
					fscanf(fp, "%d", &(interr[i]));
				for(int i = 0; i<num_proc; i++)
					sprintf(buffer, "%s%d\t", buffer, interr[i]);
				sprintf(buffer, "%d", interr[6]);
				disp(buffer);
				newl();

				fclose(fp);
				fp = fopen("/proc/interrupts", "r");
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