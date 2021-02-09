#ifndef win_add_book_h
#define win_add_book_h

#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ncurses_my_fun.h"

#include "logic.h"

void lcatch(int ch, FORM *form, FIELD *fields[19]);
void display_add_book();

#endif