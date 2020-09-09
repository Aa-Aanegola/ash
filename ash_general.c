#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_general()
{
	int is_background = 0;
	for(int i = 0; i<strlen(read_in); i++)
	{
		if(read_in[i] == '&')
		{
			read_in[i] = ' ';
			is_background = 1;
		}
	}
	clean_string(read_in);
	get_command();

	int pid = fork();

	if(pid < 0)
	{
		write(2, "ash: New process creation failed", strlen("ash: New process creation failed"));
		newl();
		return;
	}

	else if(pid == 0)
	{
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

		if(is_background)
			setpgid(0, 0);

		if(execvp(args[0], args) < 0)
		{
			write(2, "ash: Command not found", strlen("ash: Command not found"));
			newl();
			exit(0);
		}
		exit(0);
	}

	else
	{
		if(is_background)
		{
			push_child(pid);
			sleep(1);
			return;
		}
		waitpid(pid, NULL, 0);
		
	}
}