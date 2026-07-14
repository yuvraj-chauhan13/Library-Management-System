#include "loans.h"
#include "books.h"
#include "storage.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

time_t string_to_time(const char *date)
{
    struct tm tm = {0};

    strptime(date,
             "%Y-%m-%d %H:%M:%S",
             &tm);

    return mktime(&tm);
}

static int parse_loan(const char *line, Loan *loan)
{
    if (line == NULL || loan == NULL)
        return 0;

    if (sscanf(line,
               "%d|%d|%49[^|]|%19[^|]|%19[^|]|%d|%lf",
               &loan->loanId,
               &loan->bookId,
               loan->username,
               loan->borrowDate,
               loan->dueDate,
               &loan->returned,
               &loan->fine) != 7)
    {
        return 0;
    }

    return 1;
}

char *view_user_loans(const char *username)
{
    if (username == NULL)
        return strdup("Invalid username.\n");

    char *content = read_file(LOANS_FILE);

    if (content == NULL)
        return strdup("No loans found.\n");

    char *result = malloc(8192);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    Loan loan;

    int found = 0;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_loan(line, &loan))
        {
            if (strcasecmp(loan.username, username) == 0)
            {
                found = 1;

                char temp[512];

                snprintf(temp,
                         sizeof(temp),
                         "=========================================\n"
                         "Loan ID      : %d\n"
                         "Book ID      : %d\n"
                         "Borrow Date  : %s\n"
                         "Due Date     : %s\n"
                         "Status       : %s\n"
                         "Fine         : ₹%.2f\n"
                         "=========================================\n\n",
                         loan.loanId,
                         loan.bookId,
                         loan.borrowDate,
                         loan.dueDate,
                         loan.returned ? "Returned" : "Borrowed",
                         loan.fine);

                strcat(result, temp);
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    if (!found)
    {
        strcpy(result, "No loans found for this user.\n");
    }

    return result;
}

char *list_loans(void)
{
    char *content = read_file(LOANS_FILE);

    if (content == NULL)
        return strdup("No loans found.\n");

    char *result = malloc(8192);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    Loan loan;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_loan(line, &loan))
        {
            char temp[512];

            snprintf(temp,
                     sizeof(temp),
                     "=========================================\n"
                     "Loan ID      : %d\n"
                     "Book ID      : %d\n"
                     "Borrower     : %s\n"
                     "Borrow Date  : %s\n"
                     "Due Date     : %s\n"
                     "Status       : %s\n"
                     "Fine         : ₹%.2f\n"
                     "=========================================\n\n",
                     loan.loanId,
                     loan.bookId,
                     loan.username,
                     loan.borrowDate,
                     loan.dueDate,
                     loan.returned ? "Returned" : "Borrowed",
                     loan.fine);

            strcat(result, temp);
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    return result;
}

char *overdue_books(void)
{
    char *content = read_file(LOANS_FILE);

    if (content == NULL)
        return strdup("No loans found.\n");

    char *result = malloc(8192);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    strcat(result,
           "=========================================\n");
    strcat(result,
           "          OVERDUE BOOKS REPORT\n");
    strcat(result,
           "=========================================\n\n");

    Loan loan;

    int found = 0;

    time_t now = time(NULL);

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_loan(line, &loan))
        {
            if (!loan.returned)
            {
                time_t due = string_to_time(loan.dueDate);

                if (now > due)
                {
                    found = 1;

                    long daysLate = (now - due) / (24 * 60 * 60);

                    if (daysLate < 1)
                        daysLate = 1;

                    char temp[512];

                    snprintf(temp,
                             sizeof(temp),
                             "Loan ID    : %d\n"
                             "Book ID    : %d\n"
                             "User       : %s\n"
                             "Due Date   : %s\n"
                             "Days Late  : %ld\n"
                             "Fine       : ₹%.2f\n"
                             "-----------------------------------------\n",
                             loan.loanId,
                             loan.bookId,
                             loan.username,
                             loan.dueDate,
                             daysLate,
                             (double)daysLate);

                    strcat(result, temp);
                }
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    if (!found)
    {
        strcat(result,
               "No overdue books.\n");
    }

    strcat(result,
           "=========================================\n");

    return result;
}