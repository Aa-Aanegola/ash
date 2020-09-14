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
	disp(display_name);
	
	fflush(0);

	take_inp();
	int pos = 0, bre = 0;
	while(1)
	{
		// If all commands have been executed
		if(bre)
			break;

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
		
		check_redir();

		if(!strlen(read_in))
			continue;

		exec_builtin();
	}
}