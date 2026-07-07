#ifndef BOOKS_H
#define BOOKS_H

// book format in data/books.txt:
// id|title|author|total|available|popularity

char *search_books(const char *keyword); // malloc string
char *list_books(); // malloc string
int add_book(const char *title, const char *author, int copies);
int remove_book(int book_id);
int borrow_book_by_id(int book_id, const char *username); // returns loan id (>0) or -1
int return_book_by_loan(int loan_id); // returns 0 ok, -1 fail

#endif // BOOKS_H

