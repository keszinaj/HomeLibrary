#ifndef logic_h
#define logic_h

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>

struct books{
    int index_number;//jeszcze nie wykorzystałem
    char title[60];
    char author[60];
    int red;
    int num_pages;
    int stars;
    char where_is[100];
    char notes[500];
    char tag[30];
    int lended;
    char whom_l[35];
    struct books* next;
    };
typedef struct books book_t;

book_t *load_data_base();
void print_db(book_t *f_book);
void print_db_slot(book_t *f_book);
book_t *add_b(book_t *book_list);
void user_add_data(book_t *f_book);
book_t *rmv_if(book_t *book_list, int id);
book_t *save(book_t *book_list);
void save_one_book(FILE *db, book_t *book);
int number_of_books(book_t *first_book);
#endif