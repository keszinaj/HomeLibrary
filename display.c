#include <ncurses.h>
#include<form.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
void print_menu();
void display_add_book();
void lcatch(int ch, FORM *form, FIELD *fields[17]);
static char* trim_whitespaces(char *str);
void init_scr();



void init_scr()
{
	//system("resize -s 30 80");
	initscr();
    noecho();
    cbreak();
    //curs_set(0);//kursor sie nie pokaże
    print_menu();



    //upewniamy się że program poczeka zanim się zamknie
    getch();
	endwin();
}

void print_menu()
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