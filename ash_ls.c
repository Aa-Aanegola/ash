#ifndef SHELL_H
#include"Shell.h"
#endif
#ifndef VARIABLE_H
#include"Variables.h"
#endif
#ifndef FUNC_H
#include"Functions.h"
#endif

void ash_ls()
{
	int flag[3] = {0, 0, 0};
	char *token;
	char *dup_in = (char*)malloc(1000*sizeof(char));
	strcpy(dup_in, read_in);

	token = strtok(dup_in, " ");

	while(token != NULL)
	{
		token = strtok(0, " ");
		if(token == NULL)
			break;
		if(token[0] == '-')
		{
			for(int i = 1; i<strlen(token); i++)
			{
				if(token[i] == '\n')
					continue;
				if(token[i] == 'l')
					flag[0] = 1;
				else if(token[i] == 'a')
					flag[1] = 1;
				else
					flag[2] = 1;
			}
		}
	}

	if(flag[2])
	{
		write(1, "ash: ls: Invalid option", sizeof("ash: ls: Invalid option"));
		newl();
		return;
	}
	int dir_count = 0;

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		token = strtok(0, " ");
		if(token == NULL)
			break;
		if(token[0] == '-')
			continue;

		dir_count++;
		strcpy(spec_dir, token);
		if(spec_dir[strlen(spec_dir)-1] == '\n')
			spec_dir[strlen(spec_dir)-1] = '\0';

		check_dir();
		DIR *dir;

		dir = opendir(target);
		if(dir)
			closedir(dir);
		else
		{
			write(1, "ash: ls: Specified directory isn't a directory/doesn't exist", strlen("ash: ls: Specified directory isn't a directory/doesn't exist"));
			newl();
			return;
		}
	}

	if(dir_count == 0)
		sprintf(read_in, "%s .", read_in);

	strcpy(dup_in, read_in);
	token = strtok(dup_in, " ");
	while(token != NULL)
	{
		token = strtok(0, " ");
		if(token == NULL)
			break;
		if(token[0] == '-')
			continue;

		strcpy(spec_dir, token);
		if(spec_dir[strlen(spec_dir)-1] == '\n')
			spec_dir[strlen(spec_dir)-1] = '\0';

		check_dir();
		DIR *dir;

		dir = opendir(target);
		struct dirent *entry;

		if(dir)
		{	
			if(dir_count > 1)
			{
				disp(spec_dir);
				write(1, ":", 1);
				newl();
			}

			while((entry = readdir(dir)) != NULL)
			{
				if(!flag[1] && entry->d_name[0] == '.')
					continue;
				if(flag[0])
				{
					char *temp = (char*)malloc(100*sizeof(char));
					char *fname = (char*)malloc(1000*sizeof(char));
					
					sprintf(fname, "%s/%s", target, entry->d_name);
					get_perm(fname);
					disp(permissions);
					write(1, " ", 1);
					struct stat st;
					stat(fname, &st);
					sprintf(temp, "%ld", st.st_nlink);
					disp(temp);
					write(1, " ", 1);
					struct passwd *user = getpwuid(st.st_uid);
	    			struct group  *group = getgrgid(st.st_gid);
					disp(user->pw_name);
					write(1, " ", 1);
					disp(group->gr_name);
					write(1, " ", 1);
					sprintf(temp, "%ld", st.st_size);
					disp(temp);
					write(1, " ", 1);
					strftime(temp, 50, "%B %d %H:%M", localtime(&st.st_mtime));
					disp(temp);
					write(1, " " , 1);
					free(temp);
					free(fname);
				}

				disp(entry->d_name);
				newl();
			}
			closedir(dir);
			if(dir_count > 1)
				newl();
		}
	}

	free(dup_in);
}