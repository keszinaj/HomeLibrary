#ifndef ncurses_my_fun_h
#define ncurses_my_fun_h

#include <ncurses.h>
#include <string.h> 
#include<ctype.h>
#include <stdio.h>

char* trim_whitespaces(char *str);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);

#endif