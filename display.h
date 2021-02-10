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
#include "win_lent_books.h"
#include "win_search.h"
#include "win_edit_book.h"
int print_menu(book_t *first_book);
void init_scr(book_t *first_book);
void display_books(book_t *first_book);
void display_info_win();
void dispaly_ssaved_window();



extern book_t *general_first_book;
#endif