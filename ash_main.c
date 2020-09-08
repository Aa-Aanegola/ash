#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_main()
{
	disp(display_name);
		
	take_inp();
	int pos = 0, bre = 0;
	while(1)
	{
		if(bre)
			break;

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

		read_in[i] = '\0';
		clean_string(read_in);
		if(!strlen(read_in))
			continue;
		
		get_command();
		
		ash_history_write();

		if(!strcmp(command_word, "q") || !strcmp(command_word, "quit") || !strcmp(command_word, "exit"))
		{
			uflag = 1;
			return;
		}

		if(!strcmp(command_word, "clear"))
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
		else
			ash_general();
	}
}