#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"
#include "info_win.h"
#include "ncurses_my_fun.h"
#include "win_add_book.h"
#include "win_lent_books.h"
#include "win_search.h"
int print_menu(book_t *first_book);
void init_scr(book_t *first_book);
void display_books(book_t *first_book);


//chyba trzeba potem zrobić zmienna ogolna first book
book_t *general_first_book;

void init_scr(book_t *first_book)
{
	general_first_book=first_book;
	//system("resize -s 30 80");
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

int print_menu(book_t *first_book)
{
	curs_set(0);
    int xMax, yMax;
    getmaxyx(stdscr, yMax,xMax);

    WINDOW *menuwin;//pointer to a window
    menuwin=newwin(yMax-2, 24, 1, xMax-26);
    WINDOW *logowin;//pointer to a window
    logowin=newwin(yMax-2, xMax-28, 1, 2);
    box(menuwin, 0, 0);//print box
    box(logowin, 0, 0);
    refresh();
	//mvwprintw(logowin, 1,1, "_    _");
	//mvwprintw(logowin,1, 2 ,"  | |  | | ");
	//wprintw(logowin, "  | |__| | ___  _ __ ___   ___ \n");
	//wprintw(logowin, "  |  __  |/ _ \\| '_ ` _ \\ / _ \\ \n");
	//wprintw(logowin, "  | |  | | (_) | | | | | |  __/");
    wrefresh(menuwin);
    wrefresh(logowin);

//makes we can use arrow key
    keypad(menuwin, true);
    char *choices[]={
    "Books",
    "Lent books",
    "Tags",
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
        for(int i=0;i<8;i++)
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
                if(highlight==8)
                 highlight=7;
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
		return 1;
	}
	else if(highlight==1)
	{
		display_lent_books(first_book);
		return 1;
	}
	else if(highlight==3)
	{
		search_pop_up();
		return 1;
	}
	else if(highlight==4)
	{
		display_add_book();
		return 1;
	}
	else if(highlight==5)
	{
		display_info_win();
		return 1;
	}
	else if(highlight==6)
	{
		save(general_first_book);
		dispaly_ssaved_window();
		return 1;
	}
	else if(highlight==7)
	{
		return 0;
	}
    printw("your choice was %s", choices[highlight]);
}






void display_books(book_t *first_book)
{
	book_t *f_book=first_book;
	initscr();
	
        cbreak();
        noecho();
		keypad(stdscr, TRUE);
	//val
	ITEM **books;
	MENU *books_menu;
	int n_books, i, c;
	WINDOW *my_books_menu;
	//here in future will be colors edc

	//initialize item
	n_books=number_of_books(first_book)+1;
	books = (ITEM **)calloc(n_books, sizeof(ITEM *));
        for(i = 0; i < n_books; ++i)//czy nie wychodzi za book??
		{
				if(first_book==NULL)
				{
					books[i] = new_item((char *)NULL,(char *) NULL);
				}
				else
				{
               		books[i] = new_item(first_book->title, first_book->author);
					first_book=first_book->next;
				}
		}

	books_menu = new_menu((ITEM **)books);

	// Create the window to be associated with the menu and give size
        my_books_menu = newwin(20, 78, 1, 1);
        keypad(my_books_menu, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(books_menu, my_books_menu);
        set_menu_sub(books_menu, derwin(my_books_menu, 17, 76, 3, 1));//max height and width in subwindow
		set_menu_format(books_menu, 16, 1);//16 row 1 column
			
	/* Set menu mark to the string " * " */
        set_menu_mark(books_menu, " * ");

	/* Print a border around the main window and print a title */
    box(my_books_menu, 0, 0);
	print_in_middle(my_books_menu, 1, 0, 78, "Your Books");
	mvwaddch(my_books_menu, 2, 0, ACS_LTEE);
	mvwhline(my_books_menu, 2, 1, ACS_HLINE, 76);
	mvwaddch(my_books_menu, 2, 78, ACS_RTEE);
        
	/* Post the menu */
	post_menu(books_menu);
	wrefresh(my_books_menu);
	
	//attron(COLOR_PAIR(2));
	mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate ");
	mvprintw(LINES - 1, 0, "Use Enter to see more detail or use F1 to exit");
	//attroff(COLOR_PAIR(2));
	refresh();

	while((c = wgetch(my_books_menu)) != KEY_F(1))
	{       switch(c)
	        {	case KEY_DOWN:
				menu_driver(books_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(books_menu, REQ_UP_ITEM);
				break;
			case KEY_NPAGE:
				menu_driver(books_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(books_menu, REQ_SCR_UPAGE);
				break;
			case 10:
			{	
				char temp[60];
				temp[0]='\0';
				strcat(temp, item_name(current_item(books_menu)));
				display_single_book(temp, f_book);
				clrtoeol();
				pos_menu_cursor(books_menu);			
				redrawwin(my_books_menu);
				wrefresh(my_books_menu);
				refresh();
			}
				break;
			case KEY_BACKSPACE:
			{
				char str[60];
				str[0]='\0';
				strcat(str, item_name(current_item(books_menu)));
				general_first_book=rmv_if(general_first_book, str);
				clear();
				dispaly_delated_window();
			}
			return;
		}
		           
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i)
                free_item(books[i]);
	endwin();
}



