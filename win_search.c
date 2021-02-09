#include <ncurses.h>
#include<form.h>
#include <assert.h>
#include "ncurses_my_fun.h"
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
            ch=KEY_F(1);
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