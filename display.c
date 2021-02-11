#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "logic.h"
#include "info_win.h"
#include "ncurses_my_fun.h"
#include "win_add_book.h"
#include "win_lent_books.h"
#include "win_search.h"
#include "win_edit_book.h"
#include "win_book.h"





book_t *general_first_book;


int print_menu(book_t *first_book)
{
	curs_set(0);
    int xMax, yMax;
    getmaxyx(stdscr, yMax,xMax);//get max size of window

    WINDOW *menuwin;//pointer to a window
    menuwin=newwin(yMax-2, 24, 1, xMax-26);
    WINDOW *logowin;//pointer to a window
    logowin=newwin(yMax-2, xMax-28, 1, 2);
    box(menuwin, 0, 0);//print box
    box(logowin, 0, 0);
    refresh();

	//print my asci logo and other info
	int pos=4;
	 FILE* fp;
    fp = fopen("./logoasci.txt", "r");
    if (fp == NULL) {
      exit(-1);
    }

    char buffer[40];
    while (fgets(buffer, 39, fp))
    {
        // Remove newline because \n destroy box view
       buffer[strcspn(buffer, "\n")] = 0;
		mvwprintw(logowin, pos,7, buffer);
		pos++;
    }

    fclose(fp);
	
	mvwprintw(logowin, 1,5, "You have %d books.", number_of_books(general_first_book));
	mvwprintw(logowin, 2,5, "You lent %d books.", number_of_lent_books(general_first_book));

    wrefresh(menuwin);
    wrefresh(logowin);

	
    keypad(menuwin, true);
    char *choices[]={
    "Books",
    "Lent books",
    "Search",
    "Add Book",
    "Info",
	"Save",
    "Exit"
    };
    int choice;
    int highlight=0;

    while(1)
    {
        for(int i=0;i<7;i++)
        {
            if(i==highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 1, choices[i]);
            wattroff(menuwin, A_REVERSE);   
        }
        choice=wgetch(menuwin);
        switch(choice)
        {
            case KEY_UP:
            {
                 highlight--;
                 if(highlight==-1)
                 highlight=0;
                  break;
            }
            case KEY_DOWN:
            {
                highlight++;
                if(highlight==7)
                 highlight=6;
                break;
            }
            default:
                break;
        }
        if(choice==10)//enter
                break;
    }
    clear();
	if(highlight==0)
	{
		display_books(first_book);
	}
	else if(highlight==1)
	{
		display_lent_books(first_book);
	}
	else if(highlight==2)
	{
		search_pop_up();
	}
	else if(highlight==3)
	{
		display_add_book();
	}
	else if(highlight==4)
	{
		display_info_win();
	}
	else if(highlight==5)
	{
		save(general_first_book);
		dispaly_ssaved_window();
	}
	else if(highlight==6)
	{
		return 0;//exit program
	}
	return 1;
   // printw("your choice was %s", choices[highlight]);
}

void init_scr(book_t *first_book)
{
	//set global first book
	general_first_book=first_book;
	system("resize -s 24 80");
	initscr();
    noecho();
    cbreak();
	int run=1;
	while(run)
	{
      run=print_menu(general_first_book);
	  clear();
	}
	endwin();
}








