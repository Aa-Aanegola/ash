#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// Chains commands based on logical and and or
void ash_chain()
{
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
	strcpy(dup_in, read_in);

	read_in[0] = '\0';
	int pos = 0;

	// Iterate over the input string and look for @ and $
	for(int i = 0; i<=strlen(dup_in); i++)
	{
		// If these characters are found, handle them
		if(i == strlen(dup_in) || (dup_in[i] == '@' && isspace(dup_in[i+1])) || (dup_in[i] == '$' && isspace(dup_in[i+1])))
		{
			read_in[pos] = '\0';
			pos = 0;

			// Execute the command
			if(strlen(read_in))
				ash_pipe();
			if(strlen(read_in))
				ash_redir();
			if(strlen(read_in))
				ash_builtin();
			read_in[0] = '\0';

			// If it's the last command return
			if(i == strlen(dup_in))
				return;

			// If we have an and operation, and command was unsuccessful return
			if(dup_in[i] == '@' && suc_flag == 1)
				return;

			// If we have an or operation, and command was successful return
			if(dup_in[i] == '$' && suc_flag == 0)
				return;
			suc_flag = 0;
			continue;
		}
		read_in[pos++] = dup_in[i];
	}

	free(dup_in);
}