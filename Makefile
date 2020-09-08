all:
	gcc General_Functions.c -c
	gcc ash_ls.c -c
	gcc ash_pinfo.c -c
	gcc ash_cd.c -c
	gcc ash_pwd.c -c
	gcc ash_echo.c -c
	gcc ash_general.c -c
	gcc Main.c -c
	gcc ash_main.c -c
	gcc ash_history.c -c

	gcc General_Functions.o ash_main.o ash_history.o ash_general.o ash_ls.o ash_pinfo.o ash_cd.o ash_pwd.o ash_echo.o Main.o -o ash
	rm *.o