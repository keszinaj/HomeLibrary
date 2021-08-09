#include <ncurses.h>
#include "ncurses_my_fun.h"
#include "logic.h"
//set of useful simple windows

void display_single_book(char *title, book_t *first_book)
{
	book_t *bookdis=return_book_struct(title, first_book);
	if(bookdis==NULL)
		return;
	WINDOW *bookwin;
	bookwin=newwin(20, 78, 1, 1);
	//print nice window
	box(bookwin, 0, 0);

	//display content from struct
	mvwprintw(bookwin, 1, 2, "Title:");
	mvwprintw(bookwin, 1, 10, bookdis->title);
	mvwprintw(bookwin, 3, 2, "Author:");
	mvwprintw(bookwin, 3, 10, bookdis->author);
	mvwprintw(bookwin, 5, 2, "Number of pages:");
	char stringol[15];
	snprintf(stringol, 15, "%d", bookdis->num_pages);//change int to string
	mvwprintw(bookwin, 5, 20, stringol);
	mvwprintw(bookwin, 7, 2, "Where is:");
	mvwprintw(bookwin, 7, 11, bookdis->where_is);
	mvwprintw(bookwin, 9, 2, "tag:");
	mvwprintw(bookwin, 9, 7, bookdis->tag);
	mvwprintw(bookwin, 11, 2, "stars:");
	snprintf(stringol, 15, "%d", bookdis->stars);
	mvwprintw(bookwin, 11, 9, stringol);
	mvwprintw(bookwin, 13, 2, "red:");
	if(bookdis->red==1)
		mvwprintw(bookwin, 13, 7, "Yes");
	else
		mvwprintw(bookwin, 13, 7, "No");
	int yn=15;
	if(bookdis->lent==1)
	{
		mvwprintw(bookwin, yn, 2, "who lent:");
		mvwprintw(bookwin, yn, 12, bookdis->whom_l);
		yn=17;
	}
	mvwprintw(bookwin, yn, 2, "notes:");
	mvwprintw(bookwin, yn, 10, bookdis->notes);


	refresh();
	wrefresh(bookwin);


	getch();
	wclear(bookwin);
	endwin();
}



void display_info_win()
{
	WINDOW *win;
	win=newwin(20, 78, 1, 1);
	box(win, 0, 0);

	print_in_middle(win, 1, 0, 78, "Information about the program");

	//draw line
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, 76);
	mvwaddch(win, 2, 78, ACS_RTEE);

	mvwprintw(win,5, 6, "Made by keszianj.");
	mvprintw(LINES - 2, 0, "Press any key to continue.");
	wrefresh(win);

	refresh();
	getch();
}

void dispaly_ssaved_window()
{
	mvprintw(LINES/2-2,COLS/2-7, "SAVED CHANGES");
	mvprintw(LINES - 11, COLS/2-11, "Press any key to continue.");
	refresh();
	getch();
}

void dispaly_delated_window()
{
	mvprintw(LINES/2-2,COLS/2-5, "DELETED");
	mvprintw(LINES - 11, COLS/2-11, "Press any key to continue.");
	refresh();
	getch();
}
void dispaly_new_book_saved()
{
	curs_set(0);
	mvprintw(LINES/2-2,COLS/2-5, "Saved");
	mvprintw(LINES - 11, COLS/2-16, "New book added press F1 to continue.");

	refresh();
}