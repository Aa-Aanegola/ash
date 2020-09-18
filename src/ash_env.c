#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif
	
// Creates an environment variable with specified name
void env_set()
{
	// Variables for tokenization
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	char *variable = (char*)malloc(MIN_COMM*sizeof(char));
	char *value = (char*)malloc(MIN_COMM*sizeof(char));

	// Extract arguments, and if invalid display error messages
	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		write(2, "ash: setenv: No variable name specified", strlen("ash: setenv: No variable name specified"));
		newlerr();
		suc_flag = 1;
		return;
	}
	
	strcpy(variable, token);

	token = strtok(NULL, " ");

	// In case no argument was passed, set to null string
	if(token == NULL)
	{
		if(setenv(variable, "", 1) < 0)
		{
			write(2, "ash: setenv: Variable failed to set", strlen("ash: setenv: Variable failed to set"));
			newlerr();
			suc_flag = 1;
		}
		return;
	}
	strcpy(value, token);

	// Else set to value specified
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		if(setenv(variable, value, 1) < 0)
		{
			write(2, "ash: setenv: Variable failed to set", strlen("ash: setenv: Variable failed to set"));
			newlerr();
			suc_flag = 1;
		}
		return;
	}	

	// If too many parameters are passed display error message
	write(2, "ash: setenv: Too many parameters", strlen("ash: setenv: Too many parameters"));
	newlerr();
	suc_flag = 1;
}


// Remove an environment variable
void env_unset()
{
	// Variables used for tokenization
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	char *variable = (char*)malloc(MIN_COMM*sizeof(char));
	char *value = (char*)malloc(MIN_COMM*sizeof(char));

	token = strtok(dup_in, " ");
	token = strtok(NULL, " ");
		
	// In case no argument was passed
	if(token == NULL)
	{
		write(2, "ash: unsetenv: No variable name specified", strlen("ash: unsetenv: No variable name specified"));
		newlerr();
		suc_flag = 1;
		return;
	}
	
	strcpy(variable, token);

	// Otherwise unset the variable
	token = strtok(NULL, " ");
	if(token == NULL)
	{
		int a;
		if((a = unsetenv(variable)) < 0)
		{
			write(2, "ash: unsetenv: Variable failed to unset", strlen("ash: unsetenv: Variable failed to unset"));
			newlerr();
			suc_flag = 1;
		}
		return;
	}

	// If too many arguments were passed, display error message
	write(2, "ash: unsetenv: Too many parameters", strlen("ash: unsetenv: Too many parameters"));
	newlerr();
	suc_flag = 1;
}