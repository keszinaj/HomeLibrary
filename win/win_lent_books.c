#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include "../backend/logic.h"
#include "../backend/ncurses_my_fun.h"

void display_lent_books(book_t *first_book)
{
	book_t *still_first_book=first_book;
	initscr();
	
        cbreak();
        noecho();
		keypad(stdscr, TRUE);
	//val
	ITEM **books;
	MENU *books_menu;
	int n_books, i, c;
	WINDOW *my_books_menu;
	

	//initialize item
	n_books=number_of_lent_books(first_book)+1;
	books = (ITEM **)calloc(n_books, sizeof(ITEM *));
        for(i = 0; i < n_books; ++i)
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

	// Create the window 
        my_books_menu = newwin(20, 78, 1, 1);
        keypad(my_books_menu, TRUE);
     
	// Set main window and sub window 
        set_menu_win(books_menu, my_books_menu);
        set_menu_sub(books_menu, derwin(my_books_menu, 17, 76, 3, 1));//max height and width in subwindow
		set_menu_format(books_menu, 16, 1);//16 row 1 column
			

        set_menu_mark(books_menu, " * ");

	//visual changes
    box(my_books_menu, 0, 0);
	print_in_middle(my_books_menu, 1, 0, 78, "Lent Books");
	mvwaddch(my_books_menu, 2, 0, ACS_LTEE);
	mvwhline(my_books_menu, 2, 1, ACS_HLINE, 76);
	mvwaddch(my_books_menu, 2, 78, ACS_RTEE);
        
	
	post_menu(books_menu);
	wrefresh(my_books_menu);
	
	
	mvprintw(LINES - 2, 0, "Hit enter to change lent status");
	mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
	
	refresh();
//set keyboard behaviour in window
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
				book_t *book=return_book_struct(temp, still_first_book);
				if(book==NULL)
					printw("adsaidshfbsjkdfkl");
				else
					change_lent_status(book);
				clrtoeol();

				refresh();
			}
				return;
			
		}
                wrefresh(my_books_menu);
	}	

	clear();
	//free memory
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i){
                free_item(books[i]);
		}
		free(books);
	endwin();
	
}
