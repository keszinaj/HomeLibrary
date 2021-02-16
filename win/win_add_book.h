#ifndef win_add_book_h
#define win_add_book_h

#include <ncurses.h>
#include<form.h>
#include <stdlib.h>

#include "../backend/ncurses_my_fun.h"
#include "../backend/logic.h"
#include "display.h"
#include "info_win.h"
void lcatch(int ch, FORM *form, FIELD *fields[19]);
void display_add_book();

#endif