#ifndef win_search_h
#define win_search_h

#include <ncurses.h>
#include<form.h>
#include <assert.h>
#include "ncurses_my_fun.h"
static void search_input(int ch, FORM *form, FIELD *fields[3]);
void search_pop_up();
#endif