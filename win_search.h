#ifndef win_search_h
#define win_search_h

#include <ncurses.h>
#include<form.h>
#include<menu.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ncurses_my_fun.h"
#include "display.h"
#include "ncurses_my_fun.h"


void display_found(char *str);
void search_input(int ch, FORM *form, FIELD *fields[3]);
void search_pop_up();
#endif