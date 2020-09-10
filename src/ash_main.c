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
		if(!strlen(read_in))
			continue;
		
		get_command();
		
		ash_history_write();

		// If the command issued is to exit the shell
		if(!strcmp(command_word, "q") || !strcmp(command_word, "quit") || !strcmp(command_word, "exit"))
		{
			uflag = 1;
			return;
		}

		// Simple if else ladder to execute commands
		if(!strcmp(command_word, "clear") || !strcmp(command_word, "c"))
			clear_disp();
		else if(!strcmp(command_word, "echo"))
			ash_echo();
		else if(!strcmp(command_word, "pwd"))
			ash_pwd();
		else if(!strcmp(command_word, "cd"))
			ash_cd();
		else if(!strcmp(command_word, "ls"))
			ash_ls();
		else if(!strcmp(command_word, "pinfo"))
			ash_pinfo();
		else if(!strcmp(command_word, "history"))
			ash_history_read();
		else if(!strcmp(command_word, "nightswatch"))
			ash_watch();
		else if(!strcmp(command_word, "list"))
			back_list();
		else
			ash_general();
	}
}