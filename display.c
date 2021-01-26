#include <ncurses.h>
void print_menu();
int main(void){	
    //system("resize -s 30 80");
	initscr();
    noecho();
    cbreak();

    print_menu();



    //upewniamy się że program poczeka zanim się zamknie
    getch();
	endwin();			
	return 0;
}


void print_menu()
{
    int xMax, yMax;
    getmaxyx(stdscr, yMax,xMax);

    WINDOW *menuwin;//pointer to a window
    menuwin=newwin(yMax-2, 24, 1, xMax-26);
    WINDOW *logowin;//pointer to a window
    logowin=newwin(yMax-2, xMax-28, 1, 2);
    box(menuwin, 0, 0);//print box
    box(logowin, 0, 0);
    refresh();
    wrefresh(menuwin);
    wrefresh(logowin);

//makes we can use arrow key
    keypad(menuwin, true);
    char *choices[]={
    "Books",
    "Lended books",
    "Tags",
    "Search",
    "Add Book",
    "Delate Book",
    "Info"
    };
    int choice;
    int highlight=0;

    while(1)
    {
        for(int i=0;i<7;i++)
        {
            if(i==highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 1, choices[i]);
            wattroff(menuwin, A_REVERSE);   
        }
        choice=wgetch(menuwin);
        switch(choice)
        {
            case KEY_UP:
            {
                 highlight--;
                 if(highlight==-1)
                 highlight=0;
                  break;
            }
                 case KEY_DOWN:
            {
                highlight++;
                if(highlight==7)
                 highlight=6;
                break;
            }
            default:
                break;
        }
        if(choice==10)//enter
                break;
    }
    clear();
    printw("your choice was %s", choices[highlight]);
}