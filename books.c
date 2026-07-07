#define _GNU_SOURCE
#include "books.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define BOOKS_FILE "data/books.txt"
#define LOANS_FILE "data/loans.txt"

// -----------------------------------------
// helper: format time as YYYY-MM-DD HH:MM:SS
// -----------------------------------------
static void format_time(time_t t, char *buffer, size_t size) {
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
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
char *search_books(const char *keyword){
    char *s = read_file(BOOKS_FILE);
    if(!s) return strdup("No books available.\n");

    char *out = malloc(8192);
    out[0] = '\0';

    char *save = s;
    char *line = strtok(s, "\n");
    while(line){
        if(strcasestr(line, keyword)){
            strcat(out, line);
            strcat(out, "\n");
        }
        line = strtok(NULL, "\n");
    }
    free(save);

    if(strlen(out) == 0){
        free(out);
        return strdup("No matching books.\n");
    }
    return out;
}

// -----------------------------------------
// list books
// -----------------------------------------
char *list_books(){
    char *s = read_file(BOOKS_FILE);
    if(!s) return strdup("No books.\n");
    return s;
}

// -----------------------------------------
// add book
// -----------------------------------------
int add_book(const char *title, const char *author, int copies){
    int id = next_book_id();
    char line[2048];

    snprintf(line, sizeof(line),
             "%d|%.255s|%.255s|%d|%d|0",
             id, title, author, copies, copies);

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

                if(available > 0) available--;

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
int return_book_by_loan(int loan_id){
    char *s = read_file(LOANS_FILE);
    if(!s) return -1;

    char *out = malloc(strlen(s) + 4096);
    out[0] = '\0';

    char *save = s;
    char *line = strtok(s, "\n");

    int found = 0;
    int bookid_for_loan = 0;

    while(line){
        int id, bid, returned;
        char user[128];
        long borrow_ts, due_ts;
        double fine;

        if(sscanf(line, "%d|%d|%127[^|]|%ld|%ld|%d|%lf",
                  &id, &bid, user, &borrow_ts, &due_ts, &returned, &fine) == 7)
        {
            if(id == loan_id && returned == 0){
                found = 1;
                bookid_for_loan = bid;

                // --- NEW FINE SYSTEM ---
                time_t now = time(NULL);
                double new_fine = 0.0;

                if(now > due_ts){
                    long diff = now - due_ts;  // seconds late
                    long days_late = diff / 86400; // seconds per day
                    if(days_late < 1) days_late = 1; // minimum 1 day
                    new_fine = days_late * 1.0; // ₹1 per day (change if you want)
                }

                char tmp[1024];
                snprintf(tmp, sizeof(tmp),
                         "%d|%d|%s|%ld|%ld|1|%.2f\n",
                         id, bid, user, borrow_ts, due_ts, new_fine);

                strcat(out, tmp);
            }
            else {
                strcat(out, line);
                strcat(out, "\n");
            }
        }
        else {
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

    if(rewrite_file_atomic(LOANS_FILE, out) != 0){
        free(out);
        return -1;
    }
    free(out);

    // ---------------------------------------------
    // Increase availability in books file
    // ---------------------------------------------
    char *b = read_file(BOOKS_FILE);
    if(!b) return 0;

    char *bout = malloc(strlen(b) + 4096);
    bout[0] = '\0';

    char *bsave = b;
    char *bline = strtok(b, "\n");

    while(bline){
        int id, total, available, pop;
        char title[256], author[256];

        if(sscanf(bline, "%d|%255[^|]|%255[^|]|%d|%d|%d",
                  &id, title, author, &total, &available, &pop) == 6)
        {
            if(id == bookid_for_loan){
                available++;

                char tmp[2048];
                snprintf(tmp, sizeof(tmp),
                         "%d|%s|%s|%d|%d|%d\n",
                         id, title, author, total, available, pop);

                strcat(bout, tmp);
            } else {
                strcat(bout, bline);
                strcat(bout, "\n");
            }
        }
        else {
            strcat(bout, bline);
            strcat(bout, "\n");
        }

        bline = strtok(NULL, "\n");
    }

    free(bsave);
    int rr = rewrite_file_atomic(BOOKS_FILE, bout);
    free(bout);

    return (rr == 0 ? 0 : -1);
}

