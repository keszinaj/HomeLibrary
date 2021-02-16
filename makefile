#program POZMIENIEAJ TUTTAJ na lepsze
hlib: main.o win_book.o win_edit_book.o win_search.o win_lent_books.o win_add_book.o ncurses_my_fun.o info_win.o display.o logic.o
	gcc main.o win_book.o win_edit_book.o win_search.o win_lent_books.o win_add_book.o ncurses_my_fun.o info_win.o display.o logic.o -o hlib -lform -lncurses -lmenu 
logic.o: ./backend/logic.c ./backend/logic.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./backend/logic.c
display.o: ./win/display.c ./win/display.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/display.c -lform -lncurses -lmenu 
info_win.o: ./win/info_win.c ./win/info_win.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/info_win.c -lncurses 
ncurses_my_fun.o: ./backend/ncurses_my_fun.c ./backend/ncurses_my_fun.h
	gcc -c  ./backend/ncurses_my_fun.c -lncurses 
win_add_book.o: ./win/win_add_book.c ./win/win_add_book.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/win_add_book.c -lform -lncurses -lmenu 
win_lent_books.o: ./win/win_lent_books.c ./win/win_lent_books.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/win_lent_books.c -lform -lncurses -lmenu 
win_search.o: ./win/win_search.c ./win/win_search.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/win_search.c -lform -lncurses 
win_edit_book.o: ./win/win_edit_book.c ./win/win_edit_book.h
	gcc -c  -std=c11 -Wall -Wextra -Werror -lm ./win/win_edit_book.c -lform -lncurses 
win_book.o: ./win/win_book.c ./win/win_book.h
	gcc -c -std=c11 -Wall -Wextra -Werror -lm ./win/win_book.c -lform -lncurses -lmenu 
main.o: main.c ./win/display.h ./backend/logic.h
	gcc -c -std=c11 -Wall -Wextra -Werror -lm main.c 
clean: 
	rm *.o hlib