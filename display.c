#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"

int print_menu(book_t *first_book);
void display_add_book();
void lcatch(int ch, FORM *form, FIELD *fields[17]);
static char* trim_whitespaces(char *str);
void init_scr(book_t *first_book);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);
void display_books(book_t *first_book);
void display_lent_books(book_t *first_book);
void display_single_book(char *title, book_t *first_book);

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
	mvwprintw(logowin, 1,1, "_    _");
	mvwprintw(logowin,1, 2 ,"  | |  | | ");
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
	else if(highlight==4)
	{
		display_add_book();
		return 1;
	}
	else if(highlight==6)
	{
		save(general_first_book);
	}
	else if(highlight==7)
	{
		return 0;
	}
    printw("your choice was %s", choices[highlight]);
}
static char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end+1) = '\0'; //is it good idea???

	return str;
}

void lcatch(int ch, FORM *form, FIELD *fields[17])
{
	int i;

	switch (ch) {
		case KEY_F(2):
			// Or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			move(LINES-3, 2);

			for (i = 0; fields[i]; i++) {
				printw("%s", printw("%s", trim_whitespaces(field_buffer(fields[i], 0))));

				if (field_opts(fields[i]) & O_ACTIVE)
					printw("\"\t");
				else
					printw(": \"");
			}

			refresh();
			pos_form_cursor(form);
			break;

		case KEY_DOWN:
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;

		case KEY_UP:
			form_driver(form, REQ_PREV_FIELD);
			form_driver(form, REQ_END_LINE);
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

	refresh();
}
void display_add_book()
{
	clear();
	curs_set(1);
	keypad(stdscr, true);
	FIELD *fields[17];
	FORM *myForm;
	int pom_pos=2;
	for(int i=0;i<16;i++)
	{
		fields[i]=new_field(1,20,pom_pos,2,0,0);
		i++;
		fields[i]=new_field(1,50,pom_pos,17,0,0);
		pom_pos+=2;
	}
	fields[16]=NULL;//as docs say

	set_field_buffer(fields[0],0, "Title:");
	set_field_buffer(fields[2],0, "Author:");
	set_field_buffer(fields[4],0, "Red [yes/no]:");
	set_field_buffer(fields[6],0, "Pages:");
	set_field_buffer(fields[8],0, "Stars[1-6]:");
	set_field_buffer(fields[10],0, "Where is:");
	set_field_buffer(fields[12],0, "Notes:");
	set_field_buffer(fields[14],0, "Landed[yes/no]:");
	//set_field_buffer(fields[16],0, "Landed[yes/no]:");

	for(int i=1;i<16;i=i+2)
	{
		set_field_back(fields[i], A_UNDERLINE);
	}
	for(int i=0;i<16;i++)
	{
		set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
		i++;
		set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
	}

	myForm=new_form(fields);
	post_form(myForm);
	mvprintw(20, 30, "f1 to cancl f2 to save");
	curs_set(1);
    refresh();
	int ch;
	while((ch=getch())!= KEY_F(1))
		lcatch(ch, myForm, fields);
	getch();
	unpost_form(myForm);
	free_form(myForm);
	for(int i=0;i<17;i++)
	{
		free_field(fields[i]);
	}
	clear();
	return;

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

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	//watrron
	mvwprintw(win, y, x, "%s", string);
	//watroff
	refresh();

	
}
void display_single_book(char *title, book_t *first_book)
{
	clear;
	book_t *bookdis=return_book_struct(title, first_book);
	if(bookdis==NULL)
		return;
	WINDOW *bookwin;
	bookwin=newwin(20, 78, 1, 1);
	//print nice window
	box(bookwin, 0, 0);
	print_in_middle(bookwin, 1, 0, 78, bookdis->title);
	mvwaddch(bookwin, 2, 0, ACS_LTEE);
	mvwhline(bookwin, 2, 1, ACS_HLINE, 76);
	mvwaddch(bookwin, 2, 78, ACS_RTEE);
	//display content from struct
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


	//getch();
	
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

