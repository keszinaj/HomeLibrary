#include "display.h"
#include "logic.h"

int main()
{
    //load database
    book_t *data_base_list=NULL;
    data_base_list=load_data_base();
    //init program
    init_scr(data_base_list);
    return 0;
}