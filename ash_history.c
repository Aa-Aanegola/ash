#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_history_read()
{
	int space = 0;
	for(int i = 0; i<strlen(read_in); i++)
		if(read_in[i] == ' ')
			space++;
	if(space > 1)
	{
		write(1, "ash: history: invalid arguments", strlen("ash: history: invalid arguments"));
		newl();
		return;
	}

	char *path = (char*)malloc(200*sizeof(char));
	sprintf(path, "/proc/%d/exe", getpid());
	char *executable = (char*)malloc(200*sizeof(char));
	for(int i = 0; i<200; i++)
		executable[i] = '\0';
	readlink(path, executable, 200);

	for(int i = strlen(executable)-1; i>=0; i--)
	{
		if(executable[i] == '/')
			break;
		else
			executable[i] = '\0';
	}

	strcat(executable, "history");

	FILE *read_file = fopen(executable, "a+");
	char *buffer = (char*)malloc(100*sizeof(char));

	int c = 10;
	
	if(space)
	{
		char *dup_in = (char*)malloc(1000*sizeof(char));
		strcpy(dup_in, read_in);

		char *token;
		token = strtok(dup_in, " ");
		token = strtok(0, " ");

		c = atoi(token);

		if(!c)
		{
			write(1, "ash: history: invalid argument", strlen("ash: history: invalid argument"));
			newl();
			return;
		}
	}


	while(fgets(buffer,  1000, read_file)!=NULL && c)
	{
		if(!strcmp(buffer, "\n"))
			continue;
		disp(buffer);
		c--;
	}

	fclose(read_file);

	free(path);
	free(executable);
}

void ash_history_write()
{
	char *path = (char*)malloc(200*sizeof(char));
	sprintf(path, "/proc/%d/exe", getpid());
	char *read_executable = (char*)malloc(200*sizeof(char));
	char *write_executable = (char*)malloc(200*sizeof(char));

	for(int i = 0; i<200; i++)
		read_executable[i] = '\0';
	readlink(path, read_executable, 200);

	for(int i = strlen(read_executable)-1; i>=0; i--)
	{
		if(read_executable[i] == '/')
			break;
		else
			read_executable[i] = '\0';
	}

	strcpy(write_executable, read_executable);
	strcat(read_executable, "history");

	FILE *read_file = fopen(read_executable, "a+");
	fclose(read_file);
	read_file = fopen(read_executable, "a+");

	strcat(write_executable, "temp");


	FILE *write_file = fopen(write_executable, "w");
	char *buffer = (char*)malloc(100*sizeof(char));

	int c = 1;
	fputs(read_in, write_file);
	fputs("\n", write_file);

	while(fgets(buffer,  1000, read_file)!=NULL && c<20)
	{
		if(!strcmp(buffer, "\n"))
			continue;
		//disp(buffer);
		fputs(buffer, write_file);
		fputs("\n", write_file);
		c++;
	}

	fclose(read_file);
	fclose(write_file);

	remove(read_executable);
	rename(write_executable, read_executable);

	free(buffer);
	free(path);
	free(read_executable);
	free(write_executable);
}