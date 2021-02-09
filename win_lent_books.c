#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"
#include "ncurses_my_fun.h"
void display_lent_books(book_t *first_book)
{
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
	n_books=number_of_lent_books(first_book)+1;
	books = (ITEM **)calloc(n_books, sizeof(ITEM *));
        for(i = 0; i < n_books; ++i)//czy nie wychodzi za book??
		{
				if(first_book==NULL)
				{
					books[i] = new_item((char *)NULL,(char *) NULL);
				}
				else
				{
					if(first_book->lent==1)
               			books[i] = new_item(first_book->title, first_book->whom_l);
					else
						i--;
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
	print_in_middle(my_books_menu, 1, 0, 78, "Lent Books");
	mvwaddch(my_books_menu, 2, 0, ACS_LTEE);
	mvwhline(my_books_menu, 2, 1, ACS_HLINE, 76);
	mvwaddch(my_books_menu, 2, 78, ACS_RTEE);
        
	/* Post the menu */
	post_menu(books_menu);
	wrefresh(my_books_menu);
	
	//attron(COLOR_PAIR(2));
	mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
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
				//display_single_book();
				move(20, 0);
				clrtoeol();
				use_default_colors();
				mvprintw(20, 0, "Item selected is : %s", 
						temp);
				pos_menu_cursor(books_menu);
				

				refresh();
			}
				break;
		}
                wrefresh(my_books_menu);
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i)
                free_item(books[i]);
	endwin();
}
