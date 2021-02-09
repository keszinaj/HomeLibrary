#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ncurses_my_fun.h"
#include "logic.h"
#include "display.h"


void lcatch(int ch, FORM *form, FIELD *fields[19])
{
	int i;

	switch (ch) {
		case KEY_F(2):
			// Or the current field buffer won't be sync with what is displayed
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			printw("Added");
			add_b(general_first_book, trim_whitespaces(field_buffer(fields[1], 0)),
			trim_whitespaces(field_buffer(fields[3], 0)),
			trim_whitespaces(field_buffer(fields[5], 0)),
			trim_whitespaces(field_buffer(fields[11], 0)),
			trim_whitespaces(field_buffer(fields[7], 0)),
			trim_whitespaces(field_buffer(fields[13], 0)),
			trim_whitespaces(field_buffer(fields[17], 0)),
			trim_whitespaces(field_buffer(fields[15], 0)),
			trim_whitespaces(field_buffer(fields[19], 0)),
			trim_whitespaces(field_buffer(fields[9], 0)));
			//must add tag
			move(LINES-3, 2);

		//	for (i = 0; i<4; i++){
			//	printw("%s", printw("%s", trim_whitespaces(field_buffer(fields[i], 0))));

			//	if (field_opts(fields[i]) & O_ACTIVE)
			//		printw("\"\t");
			//	else
			//		printw(": \"");
			//}

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
	FIELD *fields[21];
	FORM *myForm;
	int pom_pos=2;
	for(int i=0;i<20;i++)
	{
		fields[i]=new_field(1,20,pom_pos,2,0,0);
		i++;
		fields[i]=new_field(1,50,pom_pos,17,0,0);
		pom_pos+=2;
	}
	fields[20]=NULL;//as docs say

	set_field_buffer(fields[0],0, "Title:");
	set_field_buffer(fields[2],0, "Author:");
	set_field_buffer(fields[4],0, "Red [yes/no]:");
	set_field_buffer(fields[6],0, "Pages:");
	set_field_buffer(fields[8],0, "Stars[1-6]:");
	set_field_buffer(fields[10],0, "Where is:");
	set_field_buffer(fields[12],0, "Notes:");
	set_field_buffer(fields[14],0, "Tag:");
	set_field_buffer(fields[16],0, "Lent[yes/no]:");
	set_field_buffer(fields[18],0, "Whom:");

	for(int i=1;i<20;i=i+2)
	{
		set_field_back(fields[i], A_UNDERLINE);
	}
	for(int i=0;i<19;i++)
	{
		set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
		i++;
		set_field_opts(fields[i], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
	}

	myForm=new_form(fields);
	post_form(myForm);
	mvprintw(22, 32, "f1 to cancl f2 to save");
	curs_set(1);
    refresh();
	int ch;
	while((ch=getch())!= KEY_F(1))
		lcatch(ch, myForm, fields);
	getch();
	unpost_form(myForm);
	free_form(myForm);
	for(int i=0;i<21;i++)
	{
		free_field(fields[i]);
	}
	clear();
	return;

}