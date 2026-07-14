#include "statistics.h"
#include "storage.h"
#include "users.h"
#include "books.h"
#include "loans.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOKS_FILE "data/books.txt"
typedef struct
{
    char title[100];
    int popularity;
} PopularBook;

static int parse_book(const char *line,
                      Book *book)
{
    if (line == NULL || book == NULL)
        return 0;

    return sscanf(line,
                  "%d|%99[^|]|%99[^|]|%d|%d|%d",
                  &book->id,
                  book->title,
                  book->author,
                  &book->totalCopies,
                  &book->availableCopies,
                  &book->popularity) == 6;
}

static int parse_user(const char *line,
                      User *user)
{
    if (line == NULL || user == NULL)
        return 0;

    return sscanf(line,
                  "%49[^|]|%127[^|]|%d",
                  user->username,
                  user->password,
                  &user->role) == 3;
}

static int parse_loan(const char *line,
                      Loan *loan)
{
    if (line == NULL || loan == NULL)
        return 0;

    return sscanf(line,
                  "%d|%d|%49[^|]|%19[^|]|%19[^|]|%d|%lf",
                  &loan->loanId,
                  &loan->bookId,
                  loan->username,
                  loan->borrowDate,
                  loan->dueDate,
                  &loan->returned,
                  &loan->fine) == 7;
}

char *library_statistics(void)
{
    int totalTitles = 0;
    int totalCopies = 0;
    int availableCopies = 0;

    char *content = read_file("data/books.txt");

    if (content != NULL)
    {
        Book book;

        char *save = content;
        char *line = strtok(content, "\n");

        while (line != NULL)
        {
            if (parse_book(line, &book))
            {
                totalTitles++;
                totalCopies += book.totalCopies;
                availableCopies += book.availableCopies;
            }

            line = strtok(NULL, "\n");
        }

        free(save);
    }

    int borrowedCopies = totalCopies - availableCopies;

    int students = 0;
    int librarians = 0;
    int admins = 0;

    content = read_file("data/users.txt");

    if (content != NULL)
    {
        User user;

        char *save = content;
        char *line = strtok(content, "\n");

        while (line != NULL)
        {
            if (parse_user(line, &user))
            {
                if (user.role == 1)
                    students++;
                else if (user.role == 2)
                    librarians++;
                else if (user.role == 3)
                    admins++;
            }

            line = strtok(NULL, "\n");
        }

        free(save);
    }

    int activeLoans = 0;
    int returnedLoans = 0;

    content = read_file("data/loans.txt");

    if (content != NULL)
    {
        Loan loan;

        char *save = content;
        char *line = strtok(content, "\n");

        while (line != NULL)
        {
            if (parse_loan(line, &loan))
            {
                if (loan.returned)
                    returnedLoans++;
                else
                    activeLoans++;
            }

            line = strtok(NULL, "\n");
        }

        free(save);
    }

    char *result = malloc(1024);

    if (result == NULL)
        return strdup("Memory allocation failed.\n");

    snprintf(result,
         1024,
         "========== LIBRARY STATISTICS ==========\n\n"

         "BOOKS\n"
         "----------------------------------------\n"
         "Total Titles      : %d\n"
         "Total Copies      : %d\n"
         "Available Copies  : %d\n"
         "Borrowed Copies   : %d\n\n"

         "USERS\n"
         "----------------------------------------\n"
         "Students          : %d\n"
         "Librarians        : %d\n"
         "Admins            : %d\n\n"

         "LOANS\n"
         "----------------------------------------\n"
         "Active Loans      : %d\n"
         "Returned Loans    : %d\n",

         totalTitles,
         totalCopies,
         availableCopies,
         borrowedCopies,

         students,
         librarians,
         admins,

         activeLoans,
         returnedLoans);

    return result;
}

char *top_borrowed_books(void)
{
    char *content = read_file(BOOKS_FILE);

    if (content == NULL)
        return strdup("No books found.\n");

    PopularBook books[100];
    int count = 0;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        Book book;

        if (parse_book(line, &book))
        {
            strcpy(books[count].title, book.title);
            books[count].popularity = book.popularity;
            count++;
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    // Sort descending by popularity
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (books[j].popularity > books[i].popularity)
            {
                PopularBook temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }

    char *result = malloc(4096);

    if (result == NULL)
        return strdup("Memory allocation failed.\n");

    result[0] = '\0';

    strcat(result,
           "=========================================\n");
    strcat(result,
           "         TOP BORROWED BOOKS\n");
    strcat(result,
           "=========================================\n");

    int limit = (count < 5) ? count : 5;

    for (int i = 0; i < limit; i++)
    {
        char row[256];

        snprintf(row,
                 sizeof(row),
                 "%d. %-30s %d borrows\n",
                 i + 1,
                 books[i].title,
                 books[i].popularity);

        strcat(result, row);
    }

    strcat(result,
           "=========================================\n");

    return result;
}