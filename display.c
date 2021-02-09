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

int print_menu(book_t *first_book);
void init_scr(book_t *first_book);
void display_books(book_t *first_book);
void display_lent_books(book_t *first_book);
void display_single_book(char *title, book_t *first_book);
void search_pop_up();
static void search_input(int ch, FORM *form, FIELD *fields[3]);
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
		}
		           
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(books_menu);
        free_menu(books_menu);
        for(i = 0; i < n_books; ++i)
                free_item(books[i]);
	endwin();
}


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

static void search_input(int ch, FORM *form, FIELD *fields[3])
{
	int i;

	switch (ch) {
		case 10:
			// Or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			move(LINES-3, 2);

			for (i = 0; fields[i]; i++) {

				printw("%s", trim_whitespaces(field_buffer(fields[i], 0)));

				if (field_opts(fields[i]) & O_ACTIVE)
					printw("\"\t");
				else
					printw(": \"");
			}

			refresh();
			pos_form_cursor(form);
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
	assert(win_body != NULL);
	box(win_body, 0, 0);
	win_form = derwin(win_body, 6, 48, 2, 1);
	assert(win_form != NULL);
	box(win_form, 0, 0);
	mvwprintw(win_body, 9, 2, "Press Enter to search F1 to exit");
	
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

	while ((ch = getch()) != KEY_F(1))
	{
		search_input(ch, form, fields);
		wrefresh(win_form);
	}
	unpost_form(form);
	free_form(form);
	free_field(fields[0]);
	free_field(fields[1]);
	delwin(win_form);
	delwin(win_body);

}