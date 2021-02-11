#include <ncurses.h>
#include <string.h> 
#include<ctype.h>
#include <stdio.h>


//I use this function due to strange format of data in form class from form.h
char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0)
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null at the end
	*(end+1) = '\0'; 

	return str;
}

//to print the title of window where I want
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	//watrron <-collor etc
	mvwprintw(win, y, x, "%s", string);
	//watroff
	refresh();

	
}