#ifndef BOOKS_H
#define BOOKS_H

// book format in data/books.txt:
// id|title|author|total|available|popularity

typedef struct
{
    int id;

    char title[100];

    char author[100];

    int totalCopies;

    int availableCopies;

    int popularity;

} Book;

char *search_books(const char *keyword);
char *list_books();

int add_book(const char *title,
             const char *author,
             int copies);

int remove_book(int book_id);

int borrow_book_by_id(int book_id,
                      const char *username);

int return_book_by_loan(int loan_id);

#endif