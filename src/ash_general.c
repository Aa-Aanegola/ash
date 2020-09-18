#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_general()
{
	// Flag set to 1 when the process is called with an &
	int is_background = 0;

	// Check if an & has been passed as an argument
	for(int i = 0; i<strlen(read_in); i++)
	{
		if(read_in[i] == '&')
		{
			read_in[i] = ' ';
			is_background = 1;
		}
	}

	// Remove whitespace and extract the command
	clean_string(read_in);
	get_command();

	// If & is specified, and the process pool is full display an error message
	if(is_background && num_children == POOL_SIZE)
	{
		write(2, "ash: Background process pool is full", strlen("ash: Background process pool is full"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// Create a child process
	int pid = fork();

	// In case fork fails
	if(pid < 0)
	{
		write(2, "ash: New process creation failed", strlen("ash: New process creation failed"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// If this is the child process
	else if(pid == 0)
	{
		// Tokenize the command
		int num_tok = 1;
		for(int i = 0; i<strlen(read_in); i++)
			if(read_in[i] == ' ')
				num_tok++;
		char *args[num_tok+1];
		args[num_tok] = NULL;
		for(int i = 0; i<num_tok; i++)
			args[i] = (char*)malloc(MIN_COMM*sizeof(char));
		int pos = 0, point = 0;
		for(int i = 0; i<strlen(read_in); i++)
		{
			if(read_in[i] == ' ')
			{
				args[point++][pos] = '\0';
				pos = 0;
			}
			else
				args[point][pos++] = read_in[i];
		}
		args[point][pos] = '\0';

		// If it's a background process change the group to ensure that it doesn't take over stdin
		if(is_background)
			setpgid(0, 0);

		// Execute the command
		if(execvp(args[0], args) < 0)
		{
			write(2, "ash: Command not found", strlen("ash: Command not found"));
			newlerr();
			exit(1);
		}
		exit(1);
	}

	// Parent process
	else
	{	
		// If background, add to pool and exit
		if(is_background)
		{
			push_child(pid);
			usleep(100000);
			return;
		}
		// If not background wait for the process to terminate
		
		fore_proc.pid = pid;
		strcpy(fore_proc.name, command_word);	
		waitpid(pid, NULL, WUNTRACED);
		fore_proc.pid = -1;
	}
}