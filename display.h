#ifndef display_h
#define display_h

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
#endif