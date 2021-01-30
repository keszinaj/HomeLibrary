#include "display.h"
#include "logic.h"
//"gcc main.c display.c -lform -lncurses"
int main()
{
    //load database
    book_t *data_base_list=NULL;
    data_base_list=load_data_base();

    init_scr(data_base_list);
    return 0;
}