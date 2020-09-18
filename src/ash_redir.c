#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Iterates over the input string, and checks if < > or >> are given
void ash_redir()
{
	// 4 flags for < > >> and error
	int flag[4] = {0, 0, 0, 0};
	// Variables for strtok
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	char *read_file = (char*)malloc(MIN_COMM*sizeof(char));
	char *write_file = (char*)malloc(MIN_COMM*sizeof(char));
	
	strcpy(dup_in, read_in);

	// Iterating over all the tokens in the string 
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		// Checks for input redirection, stores file name and checks if file exists
		if(!strcmp(token, "<"))
		{

			flag[0] = 1;

			token = strtok(0, " ");
			if(token == NULL || flag[0])
			{
				flag[3] = 1;
				break;
			}
			strcpy(read_file, token);
			int read_fd = open(read_file, O_RDONLY);
			if(read_fd < 0)
			{
				write(2, "ash: redir: Input file not found", strlen("ash: redir: Input file not found"));
				newlerr();
				read_in[0] = '\0';
				close(read_fd);
				suc_flag = 1;
				return;
			}
			close(read_fd);
		}

		// Checks for output redirection, stores name of file and creates the file
		else if(!strcmp(token, ">"))
		{
			flag[1] = 1;

			token = strtok(0, " ");
			if(token == NULL || flag[1])
			{
				flag[3] = 1;
				break;
			}
			strcpy(write_file, token);
			int write_fd = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			close(write_fd);
		}

		// Checks for output redirection with append, stores name of file and creates the file
		else if(!strcmp(token, ">>"))
		{
			flag[2] = 1;

			token = strtok(0, " ");
			if(token == NULL || flag[2])
			{
				flag[3] = 1;
				break;
			}
			strcpy(write_file, token);
			int write_fd = open(write_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			close(write_fd);
		}
		if(flag[1] && flag[2])
		{
			flag[3] = 1;
			break;
		}

		token = strtok(0, " ");
	}


	// In case both the output flags were provided in conjunction
	if(flag[3])
		{
			write(2, "ash: redir: Error in redirection", strlen("ash: redir: Error in redirection"));
			newlerr();
			read_in[0] = '\0';
			suc_flag = 1;
			return;
		}


	// If no redirection, go back to normal execution
	if(!flag[0] && !flag[1] && !flag[2])
		return;


	pid_t pid = fork();

	// Error in forking
	if(pid < 0)
	{
		write(2, "ash: Process creation failed", strlen("ash: Process creation failed"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// Child process - we run the command here, and kill the child to avoid having to reset streams
	else if(pid == 0)
	{
		if(flag[0])
		{
			int read_fd = open(read_file, O_RDONLY);
			dup2(read_fd, STDIN_FILENO);
			close(read_fd);
		}

		if(flag[1])
		{
			int write_fd = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(write_fd < 0)
			{
				write(2, "ash: redir: Input file not found", strlen("ash: redir: Input file not found"));
				newlerr();
				suc_flag = 1;
				read_in[0] = '\0';
			}

			dup2(write_fd, STDOUT_FILENO);
			close(write_fd);				
		}

		if(flag[2])
		{
			int write_fd = open(write_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if(write_fd < 0)
			{
				write(2, "ash: redir: Input file not found", strlen("ash: redir: Input file not found"));
				newlerr();
				suc_flag = 1;
				read_in[0] = '\0';
			}

			dup2(write_fd, STDOUT_FILENO);
			close(write_fd);
		}

		// Required? - double reading without 
		fflush(0);

		for(int i = 0; i<strlen(read_in); i++)
			if(read_in[i] == '<' || read_in[i] == '>')
				read_in[i] = '\0';
		ash_builtin();
		exit(0);
	}

	// Parent simply waits for the child created, and then frees all variables and exits the program
	else
	{
		waitpid(pid, NULL, 0);
		fflush(0);
		read_in[0] = '\0';

		free(dup_in);
		free(read_file);
		free(write_file);

		return;
	}
}