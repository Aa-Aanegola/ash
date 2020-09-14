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

	gcc *.o -o ash
	mkdir -p obj
	mv *.o obj