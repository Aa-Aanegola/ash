#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void env_set()
{
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	char *variable = (char*)malloc(MIN_COMM*sizeof(char));
	char *value = (char*)malloc(MIN_COMM*sizeof(char));

	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		write(2, "ash: setenv: No variable name specified", strlen("ash: setenv: No variable name specified"));
		newl();
		return;
	}
	
	strcpy(variable, token);

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		if(setenv(variable, "", 1) < 0)
			write(2, "ash: setenv: Variable failed to set", strlen("ash: setenv: Variable failed to set"));
		return;
	}
	strcpy(value, token);

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		if(setenv(variable, value, 1) < 0)
		{
			write(2, "ash: setenv: Variable failed to set", strlen("ash: setenv: Variable failed to set"));
			newl();
		}
		return;
	}	

	write(2, "ash: setenv: Too many parameters", strlen("ash: setenv: Too many parameters"));
	newl();
}

void env_unset()
{
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	char *variable = (char*)malloc(MIN_COMM*sizeof(char));
	char *value = (char*)malloc(MIN_COMM*sizeof(char));

	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
	
	if(token == NULL)
	{
		write(2, "ash: unsetenv: No variable name specified", strlen("ash: unsetenv: No variable name specified"));
		newl();
		return;
	}
	
	strcpy(variable, token);

	token = strtok(NULL, " ");
	if(token == NULL)
	{
		int a;
		if((a = unsetenv(variable)) < 0)
		{
			write(2, "ash: unsetenv: Variable failed to unset", strlen("ash: unsetenv: Variable failed to unset"));
			newl();
		}
		return;
	}

	write(2, "ash: unsetenv: Too many parameters", strlen("ash: unsetenv: Too many parameters"));
	newl();
}