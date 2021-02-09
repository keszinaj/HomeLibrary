#ifndef info_win_h
#define info_win_h

#include <ncurses.h>
#include "ncurses_my_fun.h"
#include "logic.h"

void display_single_book(char *title, book_t *first_book);
void display_info_win();
void dispaly_ssaved_window();

#endif