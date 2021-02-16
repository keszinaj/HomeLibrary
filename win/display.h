#ifndef display_h
#define display_h

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "../backend/logic.h"
#include "info_win.h"
#include "../backend/ncurses_my_fun.h"
#include "win_add_book.h"
#include "win_lent_books.h"
#include "win_search.h"
#include "win_edit_book.h"
#include "win_book.h"

int print_menu(book_t *first_book);
void init_scr(book_t *first_book);

extern book_t *general_first_book;

#endif