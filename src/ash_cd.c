#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_cd()
{
	if(strlen(read_in) <= 3)
	{
		write(2, "ash: cd: Too few arguments", strlen("ash: cd: Too few arguments"));
		newl();
		return;
	}

	for(int i = 3; i<strlen(read_in); i++)
		if(read_in[i] == ' ')
		{
			write(2, "ash: cd: Too many arguments", strlen("ash: cd: Too many arguments"));
			newl();
			return;
		}

	int pos = 0;
	for(int i = 3; i<strlen(read_in); i++)
		spec_dir[pos++] = read_in[i];
	spec_dir[pos] = '\0';

	check_dir();
	
	struct stat st;
	if(stat(target, &st) != 0)
	{
		write(2, "ash: cd: Path specified does not exist", strlen("ash: cd: Path specified does not exist"));
		newl();
		return;
	}

	if(!S_ISDIR(st.st_mode))
	{
		write(2, "ash: cd: Path specified is not a directory", strlen("ash: cd: Path specified is not a directory"));
		newl();
		return;
	}

	chdir(target);
	update_disp();
}