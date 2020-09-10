#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void ash_pinfo()
{	
	// Variables to store data about the process
	pid_t pid;
	char *temp = (char*)malloc(MIN_COMM*sizeof(char));			
		
	// In case no PID is mentioned
	if(strlen(read_in) == 5)
		pid = getpid();

	// Otherwise extract the PID
	else
	{
		int pos = 0;
		for(int i = 6; i<strlen(read_in); i++)
			temp[pos++] = read_in[i];
		temp[pos] = '\0';
		sscanf(temp, "%d", &pid);
	}
	
	// Check if it's a valid PID by opening the proc file
	sprintf(temp, "/proc/%d", pid);
	DIR *dir = opendir(temp);

	if(!dir)
	{
		write(2, "ash: pinfo: Process ID doesn't exist", strlen("ash: pinfo: Process ID doesn't exist"));
		newl();
		closedir(dir);
		free(temp);
		return;
	}
	closedir(dir);
	free(temp);	

	// To obtain executable we check the /proc/PID/exe file
	char *path = (char*)malloc(200*sizeof(char));
	sprintf(path, "/proc/%d/exe", pid);
	char *executable = (char*)malloc(MIN_COMM*sizeof(char));
	for(int i = 0; i<MIN_COMM; i++)
		executable[i] = '\0';
	readlink(path, executable, 200);

	// Convert it to a relative path 
	if(strlen(home_dir) < strlen(executable))
	{
		int sub_home = 1;
		for(int i = 0; i<strlen(home_dir); i++)
			if(home_dir[i] != executable[i])
				sub_home = 0;

		if(sub_home)
		{
			executable[0] = '~';
			executable[1] = '/';
			int pos = 2;
			for(int i = strlen(home_dir)+1; i<strlen(executable); i++)
				executable[pos++] = executable[i];
			executable[pos] = '\0';
		}
	}

	// Now read from the stat file to obtain status and memory
	sprintf(path, "/proc/%d/stat", pid);
	FILE *fp = fopen(path, "r");

	long long vsize, dummy;
	fscanf(fp, "%d", &pid);
	fgetc(fp);
	char status = fgetc(fp);
	while(status != ')')
		status = fgetc(fp);
	while(status  == ')')
		status = fgetc(fp);
	
	fscanf(fp, "%c %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld %lld", &status, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &vsize);

	char *out = (char*)malloc(MAX_COMM*sizeof(char));
	sprintf(out, "PID -- %d\nProcess Status -- %c\nMemory -- %lld KB\nExecutable Path -- %s", pid, status, vsize/1024, executable);
	disp(out);
	newl();

	free(path);
	free(executable);
}
