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
	// If no argument is passed with cd, display error message
	if(strlen(read_in) <= 3)
	{
		write(2, "ash: cd: Too few arguments", strlen("ash: cd: Too few arguments"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// Check if multiple arguments have been passed, and display error message
	for(int i = 3; i<strlen(read_in); i++)
		if(read_in[i] == ' ')
		{
			write(2, "ash: cd: Too many arguments", strlen("ash: cd: Too many arguments"));
			newlerr();
			suc_flag = 1;
			return;
		}

	// Extract directory name
	int pos = 0;

	
	for(int i = 3; i<strlen(read_in); i++)
		spec_dir[pos++] = read_in[i];
	spec_dir[pos] = '\0';

	check_dir();
	
	// Check if path specified exists using stat
	struct stat st;
	if(stat(target, &st) != 0)
	{
		write(2, "ash: cd: Path specified does not exist", strlen("ash: cd: Path specified does not exist"));
		newlerr();
		suc_flag = 1;
		return;
	}

	// Check if it's a directory
	if(!S_ISDIR(st.st_mode))
	{
		write(2, "ash: cd: Path specified is not a directory", strlen("ash: cd: Path specified is not a directory"));
		newlerr();
		suc_flag = 1;
		return;
	}

	getcwd(prev_dir, sizeof(prev_dir));
	// Change directory and update the display name
	chdir(target);
	update_disp();
}