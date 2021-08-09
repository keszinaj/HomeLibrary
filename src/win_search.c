#include <ncurses.h>
#include<form.h>
#include <assert.h> //to check out how it works
#include<menu.h>
#include <string.h>
#include <stdlib.h>

#include "ncurses_my_fun.h"
#include "logic.h"
#include "display.h"
#include "ncurses_my_fun.h"

void display_found(char *str)
{
    curs_set(0);
	//val
    book_t *first_book=general_first_book;
	ITEM **books;
	MENU *books_menu;
	int n_books, i, c;
	WINDOW *my_books_menu;
	//here in future will be colors edc

	//initialize item
	n_books=number_of_found_books(general_first_book, str)+1;
	books = (ITEM **)calloc(n_books, sizeof(ITEM *));
        for(i = 0; i < n_books; ++i)
		{
				if(first_book==NULL)
				{
					books[i] = new_item((char *)NULL,(char *) NULL);
				}
				else
				{
					if(strstr(first_book->title, str) != NULL)
               			books[i] = new_item(first_book->title, first_book->author);
					else
						i--;
					first_book=first_book->next;
				}
		}

	books_menu = new_menu((ITEM **)books);

	// Create the window 
        my_books_menu = newwin(20, 78, 1, 1);
        keypad(my_books_menu, TRUE);
     
	// Set main window and sub win
        set_menu_win(books_menu, my_books_menu);
        set_menu_sub(books_menu, derwin(my_books_menu, 17, 76, 3, 1));//max height and width in subwindow
		set_menu_format(books_menu, 16, 1);//16 row 1 column
			
	
        set_menu_mark(books_menu, " * ");

	// Print a border 
    box(my_books_menu, 0, 0);
	print_in_middle(my_books_menu, 1, 0, 78, "Found");
	mvwaddch(my_books_menu, 2, 0, ACS_LTEE);
	mvwhline(my_books_menu, 2, 1, ACS_HLINE, 76);
	mvwaddch(my_books_menu, 2, 78, ACS_RTEE);
        
	
	post_menu(books_menu);
	wrefresh(my_books_menu);
	

	mvprintw(LINES - 2, 0, "Use PageUp and PageDown to scoll down or up a page of items");
	mvprintw(LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
	
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
				display_single_book(temp, general_first_book);
				move(20, 0);
				clrtoeol();
				use_default_colors();
				pos_menu_cursor(books_menu);
				redrawwin(my_books_menu);
				wrefresh(my_books_menu);
				refresh();

			
			}
				break;
		}
                wrefresh(my_books_menu);
	}	

	//Free memory
        clear();
        delwin(my_books_menu);
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i){
                free_item(books[i]);
		}
		free(books);

	
}

void search_input(int ch, FORM *form, FIELD *fields[3])
{

	switch (ch) {
		case 10:
			// Or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			move(LINES-3, 2);
            clear();
            display_found(trim_whitespaces(field_buffer(fields[1], 0)));      
			break;
		case KEY_LEFT:
			form_driver(form, REQ_PREV_CHAR);
			break;

		case KEY_RIGHT:
			form_driver(form, REQ_NEXT_CHAR);
			break;
		// Delete the char before cursor
		case KEY_BACKSPACE:
		case 127:
			form_driver(form, REQ_DEL_PREV);
			break;

		// Delete the char under the cursor
		case KEY_DC:
			form_driver(form, REQ_DEL_CHAR);
			break;

		default:
			form_driver(form, ch);
			break;
	}
   
}

void search_pop_up()
{
	 FORM *form;
 FIELD *fields[3];
 WINDOW *win_body, *win_form;
	int ch;
	win_body = newwin(12, 50, 5, 5);
	//assert is fun which allows as to test program
	// in a nice way
	assert(win_body != NULL);
	box(win_body, 0, 0);
	win_form = derwin(win_body, 6, 48, 2, 1);
	assert(win_form != NULL);
	box(win_form, 0, 0);
	mvwprintw(win_body, 9, 2, "Press Enter to search");
	
	fields[0] = new_field(1, 10, 0, 0, 0, 0);
	fields[1] = new_field(1, 30, 0, 10, 0, 0);
	fields[2] = NULL;
	assert(fields[0] != NULL && fields[1] != NULL);

	set_field_buffer(fields[0], 0, "Search:");
	
	

	set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
	set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

	set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[0], A_TOP);

	form = new_form(fields);
	assert(form != NULL);
	set_form_win(form, win_form);
	set_form_sub(form, derwin(win_form, 3, 40, 2, 4));
	post_form(form);
	curs_set(1);
	pos_form_cursor(form);
	refresh();
	wrefresh(win_body);
	wrefresh(win_form);

	while ((ch = getch()) !=KEY_F(1))//27
	{
		search_input(ch, form, fields);
       if(ch==10)
            break;
        wrefresh(win_form);
        refresh();
	}
	unpost_form(form);
	free_form(form);
	free_field(fields[0]);
	free_field(fields[1]);
	delwin(win_form);
	delwin(win_body);
   
}
