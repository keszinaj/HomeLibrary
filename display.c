#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"

void print_menu(book_t *first_book);
void display_add_book();
void lcatch(int ch, FORM *form, FIELD *fields[17]);
static char* trim_whitespaces(char *str);
void init_scr(book_t *first_book);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);
void display_books(book_t *first_book);

//chyba trzeba potem zrobić zmienna ogolna first book


void init_scr(book_t *first_book)
{
	//system("resize -s 30 80");
	initscr();
    noecho();
    cbreak();
    //curs_set(0);//kursor sie nie pokaże
    print_menu(first_book);



    //upewniamy się że program poczeka zanim się zamknie
    getch();
	endwin();
}

void print_menu(book_t *first_book)
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
    wrefresh(menuwin);
    wrefresh(logowin);

//makes we can use arrow key
    keypad(menuwin, true);
    char *choices[]={
    "Books",
    "Lended books",
    "Tags",
    "Search",
    "Add Book",
    "Delate Book",
    "Info"
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
		display_books(first_book);
	if(highlight==4)
		display_add_book();
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
			{	move(20, 0);
				clrtoeol();
				use_default_colors();
				mvprintw(20, 0, "Item selected is : %s", 
						item_name(current_item(books_menu)));
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