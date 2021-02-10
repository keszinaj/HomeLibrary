#ifndef win_edit_book_h
#define win_edit_book_h

#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ncurses_my_fun.h"

#include "logic.h"

void move_from(book_t *book, int ch, FORM *form, FIELD *fields[19]);
void display_edit_book(char *str);

#endif
