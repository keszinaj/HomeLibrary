#program POZMIENIEAJ TUTTAJ na lepsze
output: main.o display.o logic.o
	gcc main.o display.o logic.o -o output -lform -lncurses -lmenu 
logic.o: logic.c logic.h
	gcc -c -std=c11 logic.c
display.o: display.c display.h
	gcc -c display.c -lform -lncurses -lmenu 
main.o: main.c 
	gcc -c main.c 
clean: 
	rm *.o output