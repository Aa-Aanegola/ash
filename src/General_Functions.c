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

void newlerr()
{
	write(2, "\n", 1);
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
	

	if(suc_flag)
		strcpy(display_name, ":(");
	else
		strcpy(display_name, ":)");
	
	suc_flag = 0;

	// If the current directory is not a subdirectory of the home directory, then display as is
	if(strlen(cur_dir) < strlen(home_dir))
	{
		sprintf(display_name,"%s <%s@%s:%s> ", display_name, passwd->pw_name, hname, cur_dir);
		return;
	}

	// If the current directory is the home directory display ~
	if(!strcmp(cur_dir, home_dir))
	{
		sprintf(display_name, "%s <%s@%s:~> ", display_name, passwd->pw_name, hname);
		return;
	}

	// Check if the current directory is a subdirectory of the homedirectory
	int sub_home = 1;
	for(int i = 0; i<strlen(home_dir); i++)
		if(home_dir[i] != cur_dir[i])
			sub_home = 0;
	
	// If it isn't then again just display the full path from root
	if(!sub_home)
	{
		sprintf(display_name,"%s <%s@%s:%s> ", display_name, passwd->pw_name, hname, cur_dir);
		return;
	}

	// If it is a subdirectory then prefix with ~/ and give relative path
	char *dir;
	dir = (char*)malloc(strlen(cur_dir+2));
	dir[0] = '~';
	dir[1] = '/';
	int pos = 2;
	for(int i = strlen(home_dir)+1; i<strlen(cur_dir); i++)
		dir[pos++] = cur_dir[i];
	dir[pos] = '\0';

	sprintf(display_name,"%s <%s@%s:%s> ", display_name, passwd->pw_name, hname, dir);


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
	getcwd(prev_dir, sizeof(prev_dir));
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
	// Convert . to the current directory
	if(!strcmp(spec_dir, "."))
	{
		getcwd(target, sizeof(target));
		return;
	}

	// Convert ~ to the home directory
	if(!strcmp(spec_dir, "~"))
	{
		strcpy(target, home_dir);
		return;
	}

	// Convert .. to the parent directory
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

	if(!strcmp(spec_dir, "-"))
	{
		strcpy(target, prev_dir);
		return;
	}

	// Otherwise copy the directory specified to the target
	strcpy(target, spec_dir);
}


void ash_builtin()
{
	clean_string(read_in);
	get_command();

	// If the command issued is to exit the shell
	if(!strcmp(command_word, "q") || !strcmp(command_word, "quit") || !strcmp(command_word, "exit"))
	{
		uflag = 1;
		return;
	}
	// Simple if else ladder to execute commands
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
	else if(!strcmp(command_word, "jobs"))
		ash_jobs();
	else if(!strcmp(command_word, "kjob"))
		ash_kjob();
	else if(!strcmp(command_word, "overkill"))
		child_kill();
	else if(!strcmp(command_word, "setenv"))
		env_set();
	else if(!strcmp(command_word, "unsetenv"))
		env_unset();
	else if(!strcmp(command_word, "bg"))
		ash_bg();
	else if(!strcmp(command_word, "fg"))
		ash_fg();
	else
		ash_general();
}