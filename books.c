#define _GNU_SOURCE
#include "books.h"
#include "storage.h"
#include "loans.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>


#define BOOKS_FILE "data/books.txt"
#define LOANS_FILE "data/loans.txt"

static int validate_title(const char *title)
{
    if (title == NULL)
        return 0;

    while (isspace((unsigned char)*title))
        title++;

    if (*title == '\0')
        return 0;

    if (strlen(title) >= sizeof(((Book *)0)->title))
        return 0;

    if (strchr(title, '|') != NULL)
        return 0;

    return 1;
}

static int validate_author(const char *author)
{
    if (author == NULL)
        return 0;

    while (isspace((unsigned char)*author))
        author++;

    if (*author == '\0')
        return 0;

    if (strlen(author) >= sizeof(((Book *)0)->author))
        return 0;

    if (strchr(author, '|') != NULL)
        return 0;

    return 1;
}

static int validate_copies(int copies)
{
    if (copies <= 0)
        return 0;

    if (copies > 1000)
        return 0;

    return 1;
}

// -----------------------------------------
// parse one book record
// -----------------------------------------
static int parse_book(const char *line, Book *book)
{
    if (line == NULL || book == NULL)
        return 0;

    if (sscanf(line,
               "%d|%99[^|]|%99[^|]|%d|%d|%d",
               &book->id,
               book->title,
               book->author,
               &book->totalCopies,
               &book->availableCopies,
               &book->popularity) != 6)
    {
        return 0;
    }

    return 1;
}

static int book_exists(const char *title,
                       const char *author)
{
    char *content = read_file(BOOKS_FILE);

    if (content == NULL)
        return 0;

    Book book;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_book(line, &book))
        {
            if (strcasecmp(book.title, title) == 0 &&
                strcasecmp(book.author, author) == 0)
            {
                free(save);
                return 1;
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    return 0;
}

// -----------------------------------------
// helper: format time as YYYY-MM-DD HH:MM:SS
// -----------------------------------------
static void format_time(time_t t, char *buffer, size_t size) {
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}


// -----------------------------------------
// next loan id
// -----------------------------------------
static int next_loan_id(){
    char *s = read_file(LOANS_FILE);
    int max = 9000;

    if(!s) return max + 1;

    char *save = s;
    char *line = strtok(s, "\n");

    while(line){
        int id;
        if(sscanf(line, "%d|", &id) == 1)
            if(id > max) max = id;
        line = strtok(NULL, "\n");
    }
    free(save);
    return max + 1;
}


// -----------------------------------------
// helper: next book id
// -----------------------------------------
static int next_book_id(){
    char *s = read_file(BOOKS_FILE);
    int max = 1000;
    if(!s) return max + 1;

    char *save = s;
    char *line = strtok(s, "\n");
    while(line){
        int id;
        if(sscanf(line, "%d|", &id) == 1)
            if(id > max) max = id;
        line = strtok(NULL, "\n");
    }
    free(save);
    return max + 1;
}

// -----------------------------------------
// search books
// -----------------------------------------
char *search_books(const char *keyword)
{
    char *content = read_file(BOOKS_FILE);

    if (content == NULL)
        return strdup("No books available.\n");

    char *result = malloc(8192);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    Book book;
    int found = 0;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_book(line, &book))
        {
            if (strcasestr(book.title, keyword) ||
                strcasestr(book.author, keyword))
            {
                char row[256];

                snprintf(row,
                         sizeof(row),
                         "ID: %d\n"
                         "Title: %s\n"
                         "Author: %s\n"
                         "Available: %d/%d\n"
                         "Popularity: %d\n"
                         "------------------------------------------------------------\n",
                         book.id,
                         book.title,
                         book.author,
                         book.availableCopies,
                         book.totalCopies,
                         book.popularity);

                strcat(result, row);

                found = 1;
            }
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    if (!found)
    {
        free(result);
        return strdup("No matching books found.\n");
    }

    return result;
}

char *list_books()
{
    char *content = read_file(BOOKS_FILE);

    if (content == NULL)
        return strdup("No books available.\n");

    char *result = malloc(8192);

    if (result == NULL)
    {
        free(content);
        return strdup("Memory allocation failed.\n");
    }

    result[0] = '\0';

    strcat(result,
           "====================================================================================\n");
    strcat(result,
           "                                   BOOK LIST\n");
    strcat(result,
           "====================================================================================\n");

    strcat(result,
           "ID     Title                     Author                  Available   Popularity\n");

    strcat(result,
           "------------------------------------------------------------------------------------\n");

    Book book;

    char *save = content;
    char *line = strtok(content, "\n");

    while (line != NULL)
    {
        if (parse_book(line, &book))
        {
            char row[256];

            snprintf(row,
                     sizeof(row),
                     "%-6d %-25s %-23s %3d/%-3d %8d\n",
                     book.id,
                     book.title,
                     book.author,
                     book.availableCopies,
                     book.totalCopies,
                     book.popularity);

            strcat(result, row);
        }

        line = strtok(NULL, "\n");
    }

    strcat(result,
           "====================================================================================\n");

    free(save);

    return result;
}

// -----------------------------------------
// add book
// -----------------------------------------
int add_book(const char *title,
             const char *author,
             int copies)
{
    if (!validate_title(title))
    {
        printf("Error: Invalid book title.\n");
        return -1;
    }

    if (!validate_author(author))
    {
        printf("Error: Invalid author name.\n");
        return -1;
    }

    if (!validate_copies(copies))
    {
        printf("Error: Number of copies must be between 1 and 1000.\n");
        return -1;
    }

    if (book_exists(title, author))
    {
        printf("Error: This book already exists.\n");
        return -1;
    }

    int id = next_book_id();

    char line[512];

    snprintf(line,
             sizeof(line),
             "%d|%s|%s|%d|%d|0",
             id,
             title,
             author,
             copies,
             copies);

    return append_line(BOOKS_FILE, line);
}

// -----------------------------------------
// remove book
// -----------------------------------------
int remove_book(int book_id){
    char *s = read_file(BOOKS_FILE);
    if(!s) return -1;

    char *out = malloc(strlen(s) + 2048);
    out[0] = '\0';

    char *save = s;
    char *line = strtok(s, "\n");
    int found = 0;

    while(line){
        int id;
        if(sscanf(line, "%d|", &id) == 1 && id == book_id){
            found = 1;
        } else {
            strcat(out, line);
            strcat(out, "\n");
        }
        line = strtok(NULL, "\n");
    }
    free(save);

    if(!found){
        free(out);
        return -1;
    }

    int r = rewrite_file_atomic(BOOKS_FILE, out);
    free(out);
    return r;
}

// -----------------------------------------
// borrow book
// -----------------------------------------
int borrow_book_by_id(int book_id, const char *username){
    char *s = read_file(BOOKS_FILE);
    if(!s) return -1;

    char *out = malloc(strlen(s) + 4096);
    out[0] = '\0';

    char *save = s;
    char *line = strtok(s, "\n");

    int found = 0;
    int avail = 0;

    while(line){
        int id, total, available, pop;
        char title[256], author[256];

        if(sscanf(line, "%d|%255[^|]|%255[^|]|%d|%d|%d",
                  &id, title, author, &total, &available, &pop) == 6)
        {
            if(id == book_id){
                found = 1;
                avail = available;

                if (available > 0)
                {
                    available--;
                    pop++;
                }

                char tmp[2048];
                snprintf(tmp, sizeof(tmp),
                         "%d|%.255s|%.255s|%d|%d|%d\n",
                         id, title, author, total, available, pop);
                strcat(out, tmp);
            } else {
                strcat(out, line);
                strcat(out, "\n");
            }
        } else {
            strcat(out, line);
            strcat(out, "\n");
        }
        line = strtok(NULL, "\n");
    }
    free(save);

    if(!found){
        free(out);
        return -1;
    }
    if(avail <= 0){
        free(out);
        return -2;
    }

    if(rewrite_file_atomic(BOOKS_FILE, out) != 0){
        free(out);
        return -1;
    }
    free(out);

    // REAL DATE & TIME
    time_t now = time(NULL);
    time_t due = now + 7*24*3600;

    char now_str[32], due_str[32];
    format_time(now, now_str, sizeof(now_str));
    format_time(due, due_str, sizeof(due_str));

    int loanid = next_loan_id();

    char loanline[2048];
    snprintf(loanline, sizeof(loanline),
             "%d|%d|%s|%s|%s|0|0.0",
             loanid, book_id, username,
             now_str, due_str);

    return append_line(LOANS_FILE, loanline) == 0 ? loanid : -1;
}

// -----------------------------------------
// return book
// -----------------------------------------
int return_book_by_loan(int loan_id)
{
    char *s = read_file(LOANS_FILE);
    if (!s)
        return -1;

    char *out = malloc(strlen(s) + 4096);
    out[0] = '\0';

    char *save = s;
    char *line = strtok(s, "\n");

    int found = 0;
    int bookid_for_loan = 0;

    while (line)
    {
        int id, bid, returned;
        char user[128];
        char borrowDate[20];
        char dueDate[20];
        double fine;

        if (sscanf(line,
                   "%d|%d|%127[^|]|%19[^|]|%19[^|]|%d|%lf",
                   &id,
                   &bid,
                   user,
                   borrowDate,
                   dueDate,
                   &returned,
                   &fine) == 7)
        {
            if (id == loan_id && returned == 0)
            {
                found = 1;
                bookid_for_loan = bid;

                time_t due = string_to_time(dueDate);

                time_t now = time(NULL);

                double new_fine = 0.0;

                if (now > due)
                {
                    long days = (now - due) / (24 * 60 * 60);

                    if (days < 1)
                        days = 1;

                    new_fine = days * 1.0;      // ₹1 per day
                }
                char tmp[1024];

                snprintf(tmp,
                         sizeof(tmp),
                         "%d|%d|%s|%s|%s|1|%.2f\n",
                         id,
                         bid,
                         user,
                         borrowDate,
                         dueDate,
                         new_fine);

                strcat(out, tmp);
            }
            else
            {
                strcat(out, line);
                strcat(out, "\n");
            }
        }
        else
        {
            strcat(out, line);
            strcat(out, "\n");
        }

        line = strtok(NULL, "\n");
    }

    free(save);

    if (!found)
    {
        free(out);
        return -1;
    }

    if (rewrite_file_atomic(LOANS_FILE, out) != 0)
    {
        free(out);
        return -1;
    }

    free(out);

    // ---------------------------------------------
    // Increase availability in books file
    // ---------------------------------------------
    char *b = read_file(BOOKS_FILE);

    if (!b)
        return 0;

    char *bout = malloc(strlen(b) + 4096);
    bout[0] = '\0';

    char *bsave = b;
    char *bline = strtok(b, "\n");

    while (bline)
    {
        int id, total, available, pop;
        char title[256], author[256];

        if (sscanf(bline,
                   "%d|%255[^|]|%255[^|]|%d|%d|%d",
                   &id,
                   title,
                   author,
                   &total,
                   &available,
                   &pop) == 6)
        {
            if (id == bookid_for_loan)
            {
                available++;

                char tmp[2048];

                snprintf(tmp,
                         sizeof(tmp),
                         "%d|%s|%s|%d|%d|%d\n",
                         id,
                         title,
                         author,
                         total,
                         available,
                         pop);

                strcat(bout, tmp);
            }
            else
            {
                strcat(bout, bline);
                strcat(bout, "\n");
            }
        }
        else
        {
            strcat(bout, bline);
            strcat(bout, "\n");
        }

        bline = strtok(NULL, "\n");
    }

    free(bsave);

    int rr = rewrite_file_atomic(BOOKS_FILE, bout);

    free(bout);

    return (rr == 0) ? 0 : -1;
}