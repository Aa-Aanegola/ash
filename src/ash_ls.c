#ifndef SHELL_H
#include"../include/Shell.h"
#endif
#ifndef VARIABLE_H
#include"../include/Variables.h"
#endif
#ifndef FUNC_H
#include"../include/Functions.h"
#endif

void disp_month(char *month);

void ash_ls()
{
	int flag[3] = {0, 0, 0};
	int size[5];

	char fname[MAX_COMM];
	char temp[MIN_COMM];

	char *token;
	char *dup_in = (char*)malloc(MAX_COMM*sizeof(char));
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
		write(2, "ash: ls: Invalid option", sizeof("ash: ls: Invalid option"));
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
			write(2, "ash: ls: Specified directory isn't a directory/doesn't exist", strlen("ash: ls: Specified directory isn't a directory/doesn't exist"));
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
			long long total = 0;

			if(dir_count > 1)
			{
				disp(spec_dir);
				write(1, ":", 1);
				newl();
			}

			for(int i = 0; i<5; i++)
				size[i] = 0;

			while((entry = readdir(dir)) != NULL)
			{
				if(!flag[1] && entry->d_name[0] == '.')
					continue;
				if(flag[0])
				{
					sprintf(fname, "%s/%s", target, entry->d_name);
					get_perm(fname);
					if(strlen(permissions) > size[0])
						size[0] = strlen(permissions);
					
					struct stat st;
					total += (st.st_blocks*512+1023)/1024;
					stat(fname, &st);
					sprintf(temp, "%ld", st.st_nlink);
					if(strlen(temp) > size[1])
						size[1] = strlen(temp);
					
					struct passwd *user = getpwuid(st.st_uid);
	    			sprintf(temp, "%s", user->pw_name);
					if(strlen(temp) > size[2])
						size[2] = strlen(temp);
					
					struct group  *group = getgrgid(st.st_gid);
					sprintf(temp, "%s", user->pw_name);
					if(strlen(group->gr_name) > size[3])
						size[3] = strlen(temp);
					
					sprintf(temp, "%ld", st.st_size);
					if(strlen(temp) > size[4])
						size[4] = strlen(temp);
				}
			}
			closedir(dir);
			
			dir = opendir(target);

			sprintf(temp, "total %lld\n", total);
			disp(temp);

			while((entry = readdir(dir)) != NULL)
			{
				if(!flag[1] && entry->d_name[0] == '.')
					continue;
				if(flag[0])
				{	
					sprintf(fname, "%s/%s", target, entry->d_name);
					get_perm(fname);
					disp(permissions);
					write(1, " ", 1);

					struct stat st;
					stat(fname, &st);
					sprintf(temp, "%ld", st.st_nlink);
					for(int i = 0; i<size[1]-strlen(temp); i++)
						write(1, " ", 1);	
					disp(temp);
					write(1, " ", 1);

					struct passwd *user = getpwuid(st.st_uid);
					sprintf(temp, "%s", user->pw_name);
	    			for(int i = 0; i<size[2]-strlen(temp); i++)
						write(1, " ", 1);	
					disp(user->pw_name);
					write(1, " ", 1);

					struct group  *group = getgrgid(st.st_gid);
					sprintf(temp, "%s", group->gr_name);
					for(int i = 0; i<size[3]-strlen(temp); i++)
						write(1, " ", 1);	
					disp(group->gr_name);
					write(1, " ", 1);
					
					sprintf(temp, "%ld", st.st_size);
					for(int i = 0; i<size[4]-strlen(temp); i++)
						write(1, " ", 1);
					disp(temp);
					write(1, " ", 1);

					strftime(temp, 10, "%m", localtime(&st.st_mtime));
					disp_month(temp);
					strftime(temp, 50, " %d %H:%M", localtime(&st.st_mtime));
					disp(temp);
					write(1, " " , 1);
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

void disp_month(char *month)
{
	if(!strcmp(month, "01"))
		write(1, "Jan", 3);
	else if(!strcmp(month, "02"))
		write(1, "Feb", 3);
	else if(!strcmp(month, "03"))
		write(1, "Mar", 3);
	else if(!strcmp(month, "04"))
		write(1, "Apr", 3);
	else if(!strcmp(month, "05"))
		write(1, "May", 3);
	else if(!strcmp(month, "06"))
		write(1, "Jun", 3);
	else if(!strcmp(month, "07"))
		write(1, "Jul", 3);
	else if(!strcmp(month, "08"))
		write(1, "Aug", 3);
	else if(!strcmp(month, "09"))
		write(1, "Sep", 3);
	else if(!strcmp(month, "10"))
		write(1, "Oct", 3);
	else if(!strcmp(month, "11"))
		write(1, "Nov", 3);
	else if(!strcmp(month, "12"))
		write(1, "Dec", 3);
}