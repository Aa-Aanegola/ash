#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

// Returns the present directory
void ash_pwd()
{
	getcwd(cur_dir, sizeof(cur_dir));
	disp(cur_dir);
	newl();
}