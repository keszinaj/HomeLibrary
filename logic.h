#ifndef logic_h
#define logic_h

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>
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
    int lent;
    char whom_l[35];
    struct books* next;
    };
typedef struct books book_t;

book_t *load_data_base();
void print_db(book_t *f_book);
void print_db_slot(book_t *f_book);
book_t *add_b(book_t *book_list, char *title, char *author, char *red, char *wis, char *np, char *notes, char *le, char *tag, char *whom, char *star);
void user_add_data(book_t *f_book,char *title, char *author, char *red, char *wis, char *np, char *notes, char *le, char *tag, char *whom, char *star);
book_t *rmv_if(book_t *book_list, char *str);
book_t *save(book_t *book_list);
void save_one_book(FILE *db, book_t *book);
int number_of_books(book_t *first_book);
int number_of_lent_books(book_t *first_book);
book_t *return_book_struct(char *title, book_t *first_book);
int number_of_found_books(book_t *first_book, char *str);
#endif