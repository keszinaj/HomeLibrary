#ifndef display_h
#define display_h

#include <ncurses.h>
#include<form.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"

int print_menu(book_t *first_book);
void display_add_book();
void lcatch(int ch, FORM *form, FIELD *fields[19]);
static char* trim_whitespaces(char *str);
void init_scr(book_t *first_book);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);
void display_books(book_t *first_book);
void display_lent_books(book_t *first_book);
void display_single_book(char *title, book_t *first_book);
#endif