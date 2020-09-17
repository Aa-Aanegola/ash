/*
	The program execution starts here, main has only function calls and the exit condition.
*/

#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

int main()
{
	master_pid = getpid();
	fore_proc.pid = -1;

	// All the intializer functions are called to set up. These functions are called only once
	install_signal(SIGCHLD, child_handler);
	install_signal(SIGINT, c_handler);
	install_signal(SIGTSTP, z_handler);
	initialize_disp();
	get_home();
	clear_disp();
	init_child_proc();
	
	// Setting up the global integer variables
	uflag = 0;
	num_children = 0;
	
	while(1)
	{
		ash_main();
		if(uflag)
		{
			// Exits the program, but doesn't handle children
			child_kill();
			exit(0);
		}
	}
}