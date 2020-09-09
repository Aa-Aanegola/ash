#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_echo()
{
	char *buff;
	buff = (char*)malloc(MAX_COMM*sizeof(char));
	int pos = 0;
	for(int i = 4; i<strlen(read_in); i++)
		buff[pos++] = read_in[i];
	buff[pos] = '\0';

	clean_string(buff);
	disp(buff);
	newl();
	
	free(buff);
}