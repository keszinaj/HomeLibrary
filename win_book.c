#include<ncurses.h>
#include "logic.h"
#include "display.h"

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
        for(i = 0; i < n_books; ++i)
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

	// Create the window 
        my_books_menu = newwin(20, 78, 1, 1);
        keypad(my_books_menu, TRUE);
     
	// Set main window and sub window 
        set_menu_win(books_menu, my_books_menu);
        set_menu_sub(books_menu, derwin(my_books_menu, 17, 76, 3, 1));//max height and width in subwindow
		set_menu_format(books_menu, 16, 1);//16 row 1 column
			
	
        set_menu_mark(books_menu, " * ");

	// Print a border 
    box(my_books_menu, 0, 0);
	print_in_middle(my_books_menu, 1, 0, 78, "Your Books");
	mvwaddch(my_books_menu, 2, 0, ACS_LTEE);
	mvwhline(my_books_menu, 2, 1, ACS_HLINE, 76);
	mvwaddch(my_books_menu, 2, 78, ACS_RTEE);
        

	post_menu(books_menu);
	wrefresh(my_books_menu);
	
	//attron;
	mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate or click 'e' to edit");
	mvprintw(LINES - 1, 0, "Use Enter to see more detail or use F1 to exit or use Backspace to delete");
	//attroff;
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
            //enter - display more info about the chosen book
			case 10:
			{	
				char temp[60];
				temp[0]='\0';
				strcat(temp, item_name(current_item(books_menu)));
				display_single_book(temp, f_book);
				pos_menu_cursor(books_menu);			
				redrawwin(my_books_menu);
				wrefresh(my_books_menu);
				refresh();
				break;
            }
			case KEY_BACKSPACE: //to delete
			{
				char str[60];
				str[0]='\0';
				strcat(str, item_name(current_item(books_menu)));
                //rmv from struct
				general_first_book=rmv_if(general_first_book, str);
				clear();
				dispaly_delated_window();
                //go back to menu
                return;
			}
			
			case 101://e to edit 
			{
				char str[60];
				str[0]='\0';
				strcat(str, item_name(current_item(books_menu)));
				display_edit_book(str);
                //go back to menu
                return;
			}
				

		}
		           
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i)
                free_item(books[i]);
	endwin();
}