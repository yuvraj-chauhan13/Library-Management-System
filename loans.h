#ifndef LOANS_H
#define LOANS_H

#include <time.h>
#include "users.h"

#define LOANS_FILE "data/loans.txt"

typedef struct
{
    int loanId;
    int bookId;

    char username[USERNAME_MAX];

    char borrowDate[20];
    char dueDate[20];

    int returned;

    double fine;

} Loan;

// Borrow a book
int borrow_book_by_id(int book_id,
                      const char *username);

// Return a borrowed book
int return_book_by_loan(int loan_id);

// View all loans
char *list_loans(void);

char *overdue_books(void);

// View loans of a specific user
char *view_user_loans(const char *username);

time_t string_to_time(const char *date);

#endif