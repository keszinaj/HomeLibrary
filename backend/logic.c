#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include <string.h>

//struct which contain all info about books
struct books{
    int index_number;
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

int book_index=0; 

int indexx=0;


book_t *load_data_base()
{
    FILE *db;
    db=fopen("./database.txt", "r");
    if(db==NULL) //error handling
    {
        exit(-1);
    }

    //check if database is empty
    fseek(db, 0, SEEK_END);
    if(ftell(db)==0)
    {
        return NULL;
    }
    fseek(db, 0, SEEK_SET);

    book_t *first_book;
    first_book= (book_t *) malloc(sizeof(book_t));
    
    //load forom database to struct
    char number_string[10];
    fscanf(db, " \"%[^\"]\",", first_book->title);
    fscanf(db, " \"%[^\"]\",", first_book->author);
    fscanf(db, " \"%[^\"]\",", number_string);
    sscanf(number_string, "%d", &first_book->red);
    fscanf(db, " \"%[^\"]\",", first_book->where_is);
    fscanf(db, " \"%[^\"]\",", number_string);
    sscanf(number_string, "%d", &first_book->num_pages);
    fscanf(db, " \"%[^\"]\",", number_string);
    sscanf(number_string, "%d", &first_book->stars);
    fscanf(db, " \"%[^\"]\",", first_book->notes);
    fscanf(db, " \"%[^\"]\",", first_book->tag);
    fscanf(db, " \"%[^\"]\",", number_string);
    sscanf(number_string, "%d", &first_book->lent);
    fscanf(db, " \"%[^\"]\" ", first_book->whom_l);
    first_book->index_number=indexx;
    first_book->next=NULL;
    indexx++;


    book_t *previous_book=first_book;
    while(true)
    {
        book_t *new_book;
        new_book= (book_t *) malloc(sizeof(book_t));
        previous_book->next=new_book;
        new_book->next=NULL;

        if(fscanf(db, " \"%[^\"]\",", new_book->title)<0)
        {
            previous_book->next=NULL;
            free(new_book);
            break;
        }
        fscanf(db, " \"%[^\"]\",", new_book->author);
        fscanf(db, " \"%[^\"]\",", number_string);
        sscanf(number_string, "%d", &new_book->red);
        fscanf(db, " \"%[^\"]\",", new_book->where_is);
        fscanf(db, " \"%[^\"]\",", number_string);
        sscanf(number_string, "%d", &new_book->num_pages);
        fscanf(db, " \"%[^\"]\",", number_string);
        sscanf(number_string, "%d", &new_book->stars);
        fscanf(db, " \"%[^\"]\",", new_book->notes);
        fscanf(db, " \"%[^\"]\",", new_book->tag);
        fscanf(db, " \"%[^\"]\",", number_string);
        sscanf(number_string, "%d", &new_book->lent);
        fscanf(db, " \"%[^\"]\" ", new_book->whom_l);
        new_book->index_number=indexx;
        indexx++;
        previous_book=new_book;

    }


    fclose(db);
    return first_book;
}


void print_db_slot(book_t *f_book)
{
    printf("title %s\n", f_book->title);
    printf("Author %s\n", f_book->author);
    printf("Red: %d\n", f_book->red);
    printf("where_is: %s\n", f_book->where_is);
    printf("num_pages: %d\n", f_book->num_pages);
    printf("notes: %s\n", f_book->notes);
    printf("tag: %s\n", f_book->tag);
    printf("lent: %d\n", f_book->lent);
    printf("whom_l: %s\n", f_book->whom_l);
}

void print_db(book_t *f_book)
{
    if(f_book==NULL)
    {
        printf("pusto");
        return;
    }
    while(f_book->next!=NULL)
    {
        print_db_slot(f_book);
        f_book=f_book->next;
    }
    print_db_slot(f_book);
}



void user_add_data(book_t *f_book, char *title, char *author, char *red, char *wis, char *np, char *notes, char *le, char *tag, char *whom, char *star)
{
    int num=-1;
    strcpy(f_book->title,title);
    strcpy(f_book->author, author); 
    if(strcmp("yes\0", red)==0) 
        f_book->red=1;  
    else
        f_book->red=0;  
   strcpy(f_book->where_is, wis);  
   num=atoi(np); 
    f_book->num_pages=num; 
    strcpy(f_book->notes, notes);  
    strcpy(f_book->tag, tag); 
    num=atoi(star);
     f_book->stars=num;
    if(strcmp("yes\0", le)==0) 
    {
        f_book->lent=1;
        strcpy(f_book->whom_l, whom);
    }  
    else
    {
        f_book->lent=0; 
        strcpy(f_book->whom_l, " ");
    } 
    f_book->index_number=indexx;
    f_book->next=NULL;
    indexx++;
}


book_t *add_b(book_t *book_list, char *title, char *author, char *red, char *wis, char *np, char *notes, char *le, char *tag, char *whom, char *star)
{
    //
        book_t *new_book;
        new_book = (book_t *) malloc(sizeof(book_t));
        user_add_data(new_book, title, author, red, wis, np, notes, le, tag, whom, star);
       
        new_book->next=NULL;
        if(book_list==NULL)
            return new_book;
        book_t *f_elem =book_list;
        while(book_list->next!=NULL)
            book_list=book_list->next;
        book_list->next=new_book;
        return f_elem;
}


book_t *rmv_if(book_t *book_list, char *str)
{
    if(book_list==NULL)
    {
        return NULL;
    }
    book_t *f_book =book_list;
    if(strcmp(f_book->title, str)==0)
    {
        f_book=book_list->next;
        free(book_list);
        return f_book;
    }
    book_t *previous;
    while(book_list->next!=NULL)
    {
        previous=book_list;
        book_list=book_list->next;
        if(strcmp(book_list->title, str)==0)
        {
            previous->next=book_list->next;
            free(book_list);
            return f_book;
        }
    }
    return f_book;
}
void save_one_book(FILE *db, book_t *book)
{
    fprintf(db, "\"%s\",\"%s\",\"%d\",\"%s\",\"%d\",\"%d\",\"%s\",\"%s\",\"%d\",\"%s\" \n", book->title, book->author, book->red, book->where_is, book->num_pages, book->stars, book->notes, book->tag, book->lent, book->whom_l);
}
book_t *save(book_t *book_list)
{
    FILE *db;
    book_t *f_book =book_list;
    db=fopen("database.txt", "w+");
    if(db==NULL) 
    {
        exit(-1);
    }
    if(book_list==NULL)
    {
        fputs( "\n", db );
    }
    else{
        while(book_list!=NULL)
        {
            save_one_book(db, book_list);
            book_list=book_list->next;
        }
    }
   // fputs( "Ten tekst zostanie dopisany do podanego pliku.\n", db );
    fclose( db);
    return f_book;
}



int number_of_books(book_t *first_book)
{
    int n=0;
    while(first_book!=NULL)
    {
        n++;
        first_book=first_book->next;
    }
    return n;
}

int number_of_lent_books(book_t *first_book)
{
    int n=0;
    while(first_book!=NULL)
    {
        if(first_book->lent==1)
        {
            n++;
        }
        first_book=first_book->next;
    }
    return n;
}

book_t *return_book_struct(char *title, book_t *first_book)
{
    if(first_book==NULL)
        return NULL;
    while(first_book!=NULL)
    {
        if(strcmp(title, first_book->title)==0)
            return first_book;
        first_book=first_book->next;
    }
    return NULL;
}

int number_of_found_books(book_t *first_book, char *str)
{
    int n=0;
    while(first_book!=NULL)
    {
        if(strstr(first_book->title, str) != NULL)
        {
            n++;
        }
        first_book=first_book->next;
    }
    return n;
}


void user_change_data(book_t *f_book, char *title, char *author, char *red, char *wis, char *np, char *notes, char *le, char *tag, char *whom, char *star)
{
    int num=-1;
    strcpy(f_book->title,title);
    strcpy(f_book->author, author); 
    if(strcmp("yes\0", red)==0) 
        f_book->red=1;  
    else
        f_book->red=0;  
   strcpy(f_book->where_is, wis);  
   num=atoi(np); 
    f_book->num_pages=num; 
    strcpy(f_book->notes, notes);   
    strcpy(f_book->tag, tag); 
    num=atoi(star);
     f_book->stars=num;
    if(strcmp("yes\0", le)==0) 
    {
        f_book->lent=1;
        strcpy(f_book->whom_l, whom);
    }  
    else
    {
        f_book->lent=0; 
        strcpy(f_book->whom_l, " ");
    } 

}

void change_lent_status(book_t *book)
{
    if(book==NULL)
    {
        
    }
    if(book!=NULL)
    {
        book->lent=0;
        //strcpy(book->whom_l, " ");
    }
}
/**
"Bardzo potrzebna mi jest przyjazn Pana","Zbigniew Herbert","2","Polka nad lozkiem","182","4","listy","listy","0","cos"
"Folwark zwierzęcy ","George Orwell","7","Szafa w salonie","128","5","moje notatki","klasyka","1","Slawek"
"Folwark zwierzęcy ","George Orwell","7","Szafa w salonie","128","5","moje notatki","klasyka","1","Slawek"
"Bardzo potrzebna mi jest przyjazn Pana","Zbigniew Herbert","2","Polka nad lozkiem","182","4","listy","listy","0","cos"

**/





