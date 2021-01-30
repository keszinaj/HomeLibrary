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
    int lent;
    char whom_l[35];
    struct books* next;
    };
typedef struct books book_t;

int book_index=0; //zamien to za index!!!!!

book_t *load_data_base();
void print_db(book_t *f_book);
void print_db_slot(book_t *f_book);
book_t *add_b(book_t *book_list);
void user_add_data(book_t *f_book);
book_t *rmv_if(book_t *book_list, int id);
book_t *save(book_t *book_list);
void save_one_book(FILE *db, book_t *book);
int number_of_books(book_t *first_book);
int number_of_lent_books(book_t *first_book);
book_t *return_book_struct(char *title, book_t *first_book);
int index=0;
/**
to do:
-dodaj index~~DONE
-https://rosettacode.org/wiki/Simple_database
-zapisywanie wszystkiego w pliku~ DONE
-walidacja zapisanych danych żeby nie miały "
-szukanie
-pożyczone oraz ich edytowanie
-DEFAKTO można już zacząć zabierać się za frontend

int main()
{
    printf("Program zarządzajacy ksiazkami\n");
    printf("Wpisz\n1 - by wypisać baze danych\n2- by dopisać książke do  bazy danych -3 aby usunac ksiazke po id\n-4 by wyjsć z programu\n");
    int choice;
    book_t *data_base_list=NULL;
    data_base_list=load_data_base();


    while(true)
    {
        printf("wprowadz numer : ");
        scanf("%d", &choice);
        switch(choice)
        {

        case 1:
            {
                print_db(data_base_list);
                break;
            }
        case 2:
            {
                data_base_list=add_b(data_base_list);
                break;
            }
        case 3:
            {
                int b;
                printf(" podaj ksiązke do usuniecia");
                scanf("%d", &b);
                data_base_list=rmv_if(data_base_list, b);
                break;
            }
        case 4:
            {
                data_base_list=save(data_base_list);
                break;
            }
        case 5:
            {
                return 0;
            }

        }

    }
    data_base_list=add_b(data_base_list);
    return 0;
}
*/
book_t *load_data_base()
{
    FILE *db;
    db=fopen("./database.txt", "r");
    if(db==NULL) //error handling
    {
        printf("ERROR FILE (1)\n");
        exit(-1);
    }

    //check if database is empty
    fseek(db, 0, SEEK_END);
    if(ftell(db)==0)
    {
        printf("Database is empty");
        return NULL;
    }
    fseek(db, 0, SEEK_SET);

    book_t *first_book;
    first_book= (book_t *) malloc(sizeof(book_t));
    //first_book=NULL;//to dopisałem nie wiem czy to legitne

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
    first_book->index_number=index;
    first_book->next=NULL;
    index++;


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
        new_book->index_number=index;
        index++;
        previous_book=new_book;

    }


    fclose(db);
    return first_book;
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



book_t *add_b(book_t *book_list)
{
        book_t *new_book;
        new_book = (book_t *) malloc(sizeof(book_t));
        user_add_data(new_book);
        new_book->next=NULL;
        if(book_list==NULL)
            return new_book;
        book_t *f_elem =book_list;
        while(book_list->next!=NULL)
            book_list=book_list->next;
        book_list->next=new_book;
        return f_elem;
}

void user_add_data(book_t *f_book)
{
    printf("title\n" );
    scanf("%s", f_book->title);
    printf("Author \n");
    scanf("%s", f_book->author);
    printf("Red:\n");
    scanf("%d", &f_book->red);
    printf("where_is: \n");
    scanf("%s", f_book->where_is);
    printf("num_pages:\n");
    scanf("%d", &f_book->num_pages);
    printf("notes:\n");//na razie tylko jedno słowo zapisujemy trzeba to będzie później zmieniić
    scanf("%s", f_book->notes);
    printf("tag:\n");
    scanf("%s", f_book->tag);
    printf("lendt:\n");
    scanf("%d", &f_book->lent);
    printf("whom_l:\n");
    scanf("%s", f_book->whom_l);
    f_book->index_number=index;
    f_book->next=NULL;
    index++;
}

book_t *rmv_if(book_t *book_list, int id)
{
    if(book_list==NULL)
    {
        printf("lista jest pusta");
        return NULL;
    }
    book_t *f_book =book_list;
    if(book_list->index_number==id)
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
        if(book_list->index_number==id)
        {
            previous->next=book_list->next;
            free(book_list);
            return f_book;
        }
    }
    return f_book;
}

book_t *save(book_t *book_list)
{
    FILE *db;
    book_t *f_book =book_list;
    db=fopen("database.txt", "w+");
    if(db==NULL) //error handling
    {
        printf("ERROR FILE (1)\n");
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
void save_one_book(FILE *db, book_t *book)
{
    fprintf(db, "\"%s\",\"%s\",\"%d\",\"%s\",\"%d\",\"%d\",\"%s\",\"%s\",\"%d\",\"%s\" \n", book->title, book->author, book->red, book->where_is, book->num_pages, book->stars, book->notes, book->tag, book->lent, book->whom_l);
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
        if(strcmp(title, first_book->title))
            return first_book;
        first_book=first_book->next;
    }
    return NULL;
}
/**
"Bardzo potrzebna mi jest przyjazn Pana","Zbigniew Herbert","2","Polka nad lozkiem","182","4","listy","listy","0","cos"
"Folwark zwierzęcy ","George Orwell","7","Szafa w salonie","128","5","moje notatki","klasyka","1","Slawek"
"Folwark zwierzęcy ","George Orwell","7","Szafa w salonie","128","5","moje notatki","klasyka","1","Slawek"
"Bardzo potrzebna mi jest przyjazn Pana","Zbigniew Herbert","2","Polka nad lozkiem","182","4","listy","listy","0","cos"

**/


//void search_book(book_t *book_list)


