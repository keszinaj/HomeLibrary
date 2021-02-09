#ifndef display_h
#define display_h

#include <ncurses.h>
#include<form.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "logic.h"
#include "info_win.h"
#include "ncurses_my_fun.h"
#include "win_add_book.h"

int print_menu(book_t *first_book);
void init_scr(book_t *first_book);
void display_books(book_t *first_book);
void display_lent_books(book_t *first_book);
void display_single_book(char *title, book_t *first_book);
void display_info_win();
void dispaly_ssaved_window();
void search_pop_up();
static void search_input(int ch, FORM *form, FIELD *fields[3]);

extern book_t *general_first_book;
#endif