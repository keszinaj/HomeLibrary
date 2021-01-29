#program
output: main.o display.o 
	gcc main.o display.o -o output -lform -lncurses -lmenu 
display.o: display.c display.h
	gcc -c display.c -lform -lncurses -lmenu 
main.o: main.c 
	gcc -c main.c 