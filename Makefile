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

	gcc General_Functions.o ash_main.o ash_watch.o ash_history.o ash_general.o ash_ls.o ash_pinfo.o ash_cd.o ash_pwd.o ash_echo.o Main.o -o ash
	mkdir -p obj
	mv *.o obj