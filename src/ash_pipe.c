#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_pipe()
{
	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	token = strtok(dup_in, " ");
	if(!strcmp(token, "|"))
	{
		write(2, "ash: pipe: Pipe cannot read from NULL command", strlen("ash: pipe: Pipe cannot read from NULL command"));
		newl();
		return;
	}

	int pipe_count = 0;

	token = strtok(NULL, " ");
	while(token != NULL)
	{
		if(!strcmp(token, "|"))
		{
			pipe_count++;
			token = strtok(NULL, " ");
			if(token == NULL)
			{
				write(2, "ash: pipe: Pipe cannot write to NULL command", strlen("ash: pipe: Pipe cannot write to NULL command"));
				newl();
				return;
			}
		}

		token = strtok(NULL, " ");
	}

	if(!pipe_count)
		return;

	strcpy(dup_in, read_in);
	token = strtok(dup_in, "|");


	int new_in = 0;
	pid_t pid;

	for(int i = 0; i<=pipe_count; i++)
	{
		int pipes[2];
		pipe(pipes);

		strcpy(read_in, token);
		clean_string(read_in);
		token = strtok(NULL, "|");

		pid = fork();

		if(pid < 0)
		{
			write(2, "ash: Failed to spawn new process", strlen("ash: Failed to spawn new process"));
			newl();
			return;
		}
		if(pid == 0)
		{
			dup2(new_in, 0);

			if(i != pipe_count)
				dup2(pipes[1], 1);

			close(pipes[0]);
			ash_redir();
			if(strlen(read_in))
				exec_builtin();
			exit(2);
		}
		else
		{
			waitpid(pid, NULL, 0);
			close(pipes[1]);
			new_in = pipes[0];
		}
	}

	read_in[0] = '\0';
}