#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_main()
{
	update_disp();
	disp(display_name);
	
	fflush(0);

	buffer_command[0] = '\0';
	take_inp();

	if(buffer_command[0] == '\0')
	{
		uflag = 1;
		return;
	}
	clean_string(buffer_command);


	int pos = 0, bre = 0;
	while(1)
	{
		// If all commands have been executed
		if(bre)
			break;

		if(!strlen(buffer_command))
			return;

		// To parse by semicolon
		int i = 0;
		while(buffer_command[pos] == ';')
			pos++;

		for(;buffer_command[pos]; pos++)
		{
			if(buffer_command[pos] == ';')
				break;
			read_in[i++] = buffer_command[pos];
		}
		if(pos >= strlen(buffer_command))
			bre = 1;

		// After obtaining the instruction, remove whitespace and execute if it's not NULL
		read_in[i] = '\0';
		clean_string(read_in);
		ash_history_write();
		
		if(!strlen(read_in))
			continue;
	
		// If not then check for pipes		
		ash_pipe();

		if(!strlen(read_in))
			continue;

		// If not then check for redirection
		ash_redir();

		if(!strlen(read_in))
			continue;
		
		// If not then simply execute
		ash_builtin();
	}
}
