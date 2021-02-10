#program POZMIENIEAJ TUTTAJ na lepsze
output: main.o win_edit_book.o win_search.o win_lent_books.o win_add_book.o ncurses_my_fun.o info_win.o display.o logic.o
	gcc main.o win_edit_book.o win_search.o win_lent_books.o win_add_book.o ncurses_my_fun.o info_win.o display.o logic.o -o output -lform -lncurses -lmenu 
logic.o: logic.c logic.h
	gcc -c -std=c11 logic.c
display.o: display.c display.h
	gcc -c display.c -lform -lncurses -lmenu 
info_win.o: info_win.c info_win.h
	gcc -c info_win.c -lncurses 
ncurses_my_fun.o: ncurses_my_fun.c ncurses_my_fun.h
	gcc -c ncurses_my_fun.c -lncurses 
win_add_book.o: win_add_book.c win_add_book.h
	gcc -c win_add_book.c -lform -lncurses -lmenu 
win_lent_books.o: win_lent_books.c win_lent_books.h
	gcc -c win_lent_books.c -lform -lncurses -lmenu 
win_search.o: win_search.c win_search.h
	gcc -c win_search.c -lform -lncurses 
win_edit_book.o: win_edit_book.c win_edit_book.h
	gcc -c win_edit_book.c -lform -lncurses -lmenu 
main.o: main.c 
	gcc -c main.c 
clean: 
	rm *.o output