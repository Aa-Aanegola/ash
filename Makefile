all:
	gcc ./src/General_Functions.c -c
	gcc ./src/ash_ls.c -c
	gcc ./src/ash_pinfo.c -c
	gcc ./src/ash_cd.c -c
	gcc ./src/ash_pwd.c -c
	gcc ./src/ash_echo.c -c
	gcc ./src/ash_general.c -c
	gcc ./src/Main.c -c
	gcc ./src/ash_main.c -c
	gcc ./src/ash_history.c -c
	gcc ./src/ash_watch.c -c
	gcc ./src/ash_redir.c -c
	gcc ./src/ash_env.c -c
	gcc ./src/ash_jobs.c -c
	gcc ./src/ash_kjob.c -c
	gcc ./src/ash_pipe.c -c
	gcc ./src/ash_bg.c -c
	gcc ./src/ash_fg.c -c
	gcc ./src/ash_signals.c -c

	gcc *.o -o ash
	mkdir -p obj
	mv *.o obj