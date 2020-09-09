#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

// A function to clear the screen
void clear_disp()
{
	write(1, "\e[1;1H\e[2J", 10);
}


// A function that reads user input into a buffer for parsing and processing
void take_inp()
{
	fgets(buffer_command, sizeof(buffer_command), stdin);
	clean_string(buffer_command);
}


// Displays the string passed as an argument onto stdout
void disp(char *str)
{
	write(1, str, strlen(str));
}


// Displays a newline on stdout
void newl()
{
	write(1, "\n", 1);
}


// Initializes the shell prompt with the username and hostname
void initialize_disp()
{
	struct passwd *passwd;
	passwd = getpwuid(getuid());
	gethostname(hname, sizeof(hname));

	sprintf(display_name,"<%s@%s:~> ", passwd->pw_name, hname);
}


// Updates the shell prompt when directory is changed
void update_disp()
{
	struct passwd *passwd;
	passwd = getpwuid(getuid());
	gethostname(hname, sizeof(hname));

	getcwd(cur_dir, sizeof(cur_dir));
	
	if(strlen(cur_dir) < strlen(home_dir))
	{
		sprintf(display_name,"<%s@%s:%s> ", passwd->pw_name, hname, cur_dir);
		return;
	}

	if(!strcmp(cur_dir, home_dir))
	{
		sprintf(display_name, "<%s@%s:~> ", passwd->pw_name, hname);
		return;
	}

	int sub_home = 1;
	for(int i = 0; i<strlen(home_dir); i++)
		if(home_dir[i] != cur_dir[i])
			sub_home = 0;
	
	if(!sub_home)
	{
		sprintf(display_name,"<%s@%s:%s> ", passwd->pw_name, hname, cur_dir);
		return;
	}

	char *dir;
	dir = (char*)malloc(strlen(cur_dir+2));
	dir[0] = '~';
	dir[1] = '/';
	int pos = 2;
	for(int i = strlen(home_dir)+1; i<strlen(cur_dir); i++)
		dir[pos++] = cur_dir[i];
	dir[pos] = '\0';

	sprintf(display_name,"<%s@%s:%s> ", passwd->pw_name, hname, dir);

	free(dir);	
}


// Removes extra whitespace from strings (works inplace)
void clean_string(char *str)
{
	int i, pos = 0;

	for(i = 0; str[i]; ++i)
  	{
    	if(isspace(str[i]) && (i == 0 || isspace(str[i-1])))
    		;
    	else if(isspace(str[i]))
    		str[pos++] = ' ';
    	else
    		str[pos++] = str[i];
  	}

  	str[pos] = '\0';
  	pos--;
  	while(pos>=0 && (str[pos] == ' ' || str[pos] == '\t'))
  		str[pos--] = '\0';
  	str[pos+1] = '\0';
}


// Extracts the first word from any command and stores it in command_word. Used to check for validity of commands
void get_command()
{
	int pos = 0;
	for(int i = 0; read_in[i]!=' ' && read_in[i]!='\0' && read_in[i]!='\n'; i++)
		command_word[pos++] = read_in[i];
	command_word[pos] = '\0';
}


// Used to initialize the home directory, used for ~ functionality
void get_home()
{
	getcwd(home_dir, sizeof(home_dir));
}


// Stores the file permissions in the permissions string (for ls -l)
void get_perm(char *file)
{
	struct stat st;
    if(stat(file, &st) == 0)
    {
        mode_t perm = st.st_mode;
        
        permissions[0] = '-';
        if(S_ISDIR(st.st_mode))
        	permissions[0] = 'd';
        else if(S_ISLNK(st.st_mode))
        	permissions[0] = 'l';
        else if(S_ISSOCK(st.st_mode))
        	permissions[0] = 's';
        else if(S_ISCHR(st.st_mode))
        	permissions[0] = 'c';
        else if(S_ISBLK(st.st_mode))
        	permissions[0] = 'b';
        else if(S_ISFIFO(st.st_mode))
        	permissions[0] = 'p';

        permissions[1] = (perm & S_IRUSR) ? 'r' : '-';
        permissions[2] = (perm & S_IWUSR) ? 'w' : '-';
        permissions[3] = (perm & S_IXUSR) ? 'x' : '-';
        permissions[4] = (perm & S_IRGRP) ? 'r' : '-';
        permissions[5] = (perm & S_IWGRP) ? 'w' : '-';
        permissions[6] = (perm & S_IXGRP) ? 'x' : '-';
        permissions[7] = (perm & S_IROTH) ? 'r' : '-';
        permissions[8] = (perm & S_IWOTH) ? 'w' : '-';
        permissions[9] = (perm & S_IXOTH) ? 'x' : '-';
        permissions[10] = '\0';
    }
}	


// Gives the full path to the directory specified, ie changes ~ to home etc
void check_dir()
{
	if(!strcmp(spec_dir, "."))
	{
		getcwd(target, sizeof(target));
		return;
	}

	if(!strcmp(spec_dir, "~"))
	{
		strcpy(target, home_dir);
		return;
	}

	if(!strcmp(spec_dir, ".."))
	{
		getcwd(cur_dir, sizeof(cur_dir));
		
		int i = strlen(cur_dir);
		for(; i>0; i--)
			if(cur_dir[i] == '/')
				break;
		if(i == 0)
			i++;
		target[0] = '/';
		target[i] = '\0';
		for(int j = 1; j<i; j++)
			target[j] = cur_dir[j];

		return;
	}

	strcpy(target, spec_dir);
}

void init_child_proc()
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		proc_array[i].pid = -1;
		proc_array[i].name[0] = '\0';
	}
}

//
void push_child(pid_t pid)
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid == -1)
		{
			proc_array[i].pid = pid;
			strcpy(proc_array[i].name, command_word);
			num_children++;
			return;
		}
	}	
	write(2, "ash: general: Process insertion failed", strlen("ash: general: Process insertion failed"));
	newl();
}

void back_list()
{
	char *buffer = (char*)malloc(MAX_COMM*sizeof(char));
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid != -1)
		{
			sprintf(buffer, "%d\t%s\n", proc_array[i].pid, proc_array[i].name);
			disp(buffer);
		}
	}
}


// Declaration of the sigaction handler, copied from an online resource
handler* install_signal(int signum, handler* handler)
{
	struct sigaction new_action, old_action;
	
	memset(&new_action, 0, sizeof(struct sigaction));
	new_action.sa_sigaction = handler;
	sigemptyset(&new_action.sa_mask);
	
	new_action.sa_flags = SA_RESTART|SA_SIGINFO;

	if (sigaction(signum, &new_action, &old_action) < 0)
		write(2, "ash: signal encountered an error", strlen("ash: signal encountered an error"));                                                         
    return (old_action.sa_sigaction);                                           
}


// The function that is called whenever a SIGCHLD signal is received. This was done to ensure that child exit handling is asynchronous
void child_handler(int sig, siginfo_t* info, void* vp)
{
	int status;
	int pid;
	char buffer[MAX_COMM];

	int term = 0;
	while((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		for(int i = 0; i<POOL_SIZE; i++)
		{
			if(proc_array[i].pid == pid)
			{
				term = 1;
				sprintf(buffer, "\n%s with pid %d exited %s", proc_array[i].name, pid, WIFEXITED(status) == 0 ? "abnormally" : "normally");
				write(1, buffer, strlen(buffer));
				proc_array[i].pid = -1;
				break;
			}
		}
	}
	if(term)
	{
		newl();
		disp(display_name);
	}
}

void child_kill()
{
	for(int i = 0; i<POOL_SIZE; i++)
	{
		if(proc_array[i].pid != -1)
			kill(proc_array[i].pid, SIGKILL);
	}
}