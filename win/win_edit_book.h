#ifndef win_edit_book_h
#define win_edit_book_h

#include <ncurses.h>
#include<form.h>
#include "../backend/ncurses_my_fun.h"
#include "../backend/logic.h"
#include "display.h"


void move_from(book_t *book, int ch, FORM *form, FIELD *fields[19]);
void display_edit_book(char *str);

#endif
