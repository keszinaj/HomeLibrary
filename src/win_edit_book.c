#include <ncurses.h>
#include<form.h>
#include "ncurses_my_fun.h"
#include "logic.h"
#include "display.h"


void move_from(book_t *book, int ch, FORM *form, FIELD *fields[21])
{
	
//set keyboard behaviour in window
	switch (ch) {
		case KEY_F(2)://save in struct
			{
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_PREV_FIELD);
			user_change_data(book, trim_whitespaces(field_buffer(fields[1], 0)),
			trim_whitespaces(field_buffer(fields[3], 0)),
			trim_whitespaces(field_buffer(fields[5], 0)),
			trim_whitespaces(field_buffer(fields[11], 0)),
			trim_whitespaces(field_buffer(fields[7], 0)),
			trim_whitespaces(field_buffer(fields[13], 0)),
			trim_whitespaces(field_buffer(fields[17], 0)),
			trim_whitespaces(field_buffer(fields[15], 0)),
			trim_whitespaces(field_buffer(fields[19], 0)),
			trim_whitespaces(field_buffer(fields[9], 0)));
			move(LINES-3, 2);
			refresh();
			pos_form_cursor(form);
			break;
			}

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
void display_edit_book(char *str)
{
    book_t *book=return_book_struct(str, general_first_book);

	clear();
	curs_set(1);
	keypad(stdscr, true);
	//create fields
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
    char result[10]; 
	//fill up field
	set_field_buffer(fields[0],0, "Title:");
    set_field_buffer(fields[1],0, book->title);
	set_field_buffer(fields[2],0, "Author:");
    set_field_buffer(fields[3],0, book->author);
	set_field_buffer(fields[4],0, "Red [yes/no]:");
    if(book->red==1)
        set_field_buffer(fields[4],0, "yes");
    else
        set_field_buffer(fields[4],0, "no");
    set_field_buffer(fields[5],0, book->title);
	set_field_buffer(fields[6],0, "Pages:");
    sprintf(result, "%d", book->num_pages); 
    set_field_buffer(fields[7],0, result);
	set_field_buffer(fields[8],0, "Stars[1-6]:");
    sprintf(result, "%d", book->stars); 
    set_field_buffer(fields[9],0, result);
	set_field_buffer(fields[10],0, "Where is:");
    set_field_buffer(fields[11],0, book->where_is);
	set_field_buffer(fields[12],0, "Notes:");
    set_field_buffer(fields[13],0, book->notes);
	set_field_buffer(fields[14],0, "Tag:");
    set_field_buffer(fields[15],0, book->tag);
	set_field_buffer(fields[16],0, "Lent[yes/no]:");
    if(book->lent==1)
	{
        set_field_buffer(fields[17],0, "yes");
	}
    else
	{
        set_field_buffer(fields[17],0, "no");
	}
	set_field_buffer(fields[18],0, "Whom:");
    set_field_buffer(fields[19],0, book->whom_l);
//visual changes
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
	while((ch = getch()) != KEY_F(1))
		move_from(book, ch, myForm, fields);
	//free memory
	unpost_form(myForm);
	free_form(myForm);
	for(int i=0;i<21;i++){
		free_field(fields[i]);
	}
	clear();
	return;
}

