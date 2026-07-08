#include "menu.h"
#include "users.h"
#include "books.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static void clear_stdin_line(){
    int c;
    while((c = getchar()) != '\n' && c != EOF) {}
}

static void show_start_menu(){
    printf("\n===== ADVANCED LIBRARY =====\n");
    printf("1) Login\n");
    printf("2) Register\n");
    printf("3) Exit\n");
    printf("Enter choice: ");
}

static void show_student_menu(const char *username){
    printf("\n--- STUDENT MENU (user: %s) ---\n", username);
    printf("1) Search Books\n");
    printf("2) Borrow Book\n");
    printf("3) Return Book\n");
    printf("4) View My Loans\n");
    printf("5) Logout\n");
    printf("Enter choice: ");
}

static void show_librarian_menu(){
    printf("\n--- LIBRARIAN MENU ---\n");
    printf("1) Add Book\n");
    printf("2) Remove Book\n");
    printf("3) List Books\n");
    printf("4) Logout\n");
    printf("Enter choice: ");
}

static void show_admin_menu(){
    printf("\n--- ADMIN MENU ---\n");
    printf("1) View Users\n");
    printf("2) View Logs (loans)\n");
    printf("3) List Books\n");
    printf("4) Logout\n");
    printf("Enter choice: ");
}

void run_app(){
    char input[256];
    while(1){
        show_start_menu();
        if(!fgets(input, sizeof(input), stdin)) break;
        int ch = atoi(input);
        if(ch == 1){
            char username[128], password[128];
            printf("Username: ");
            if(!fgets(username, sizeof(username), stdin)) break;
            username[strcspn(username, "\r\n")] = 0;
            printf("Password: ");
            if(!fgets(password, sizeof(password), stdin)) break;
            password[strcspn(password, "\r\n")] = 0;
            int role = authenticate_user(username, password);
            if(role == 0){
                printf("Invalid credentials.\n");
            } else if(role == 1){
                // student menu loop
                int run = 1;
                while(run){
                    show_student_menu(username);
                    if(!fgets(input, sizeof(input), stdin)) { run = 0; break; }
                    int sc = atoi(input);
                    if(sc == 1){
                        printf("Enter search keyword: ");
                        if(!fgets(input, sizeof(input), stdin)) break;
                        input[strcspn(input, "\r\n")] = 0;
                        char *res = search_books(input);
                        printf("Search results:\n%s\n", res);
                        free(res);
                    } else if(sc == 2){
                        printf("Enter book id to borrow: ");
                        if(!fgets(input, sizeof(input), stdin)) break;
                        int bid = atoi(input);
                        int loanid = borrow_book_by_id(bid, username);
                        if(loanid > 0) printf("Borrow successful. Loan id: %d\n", loanid);
                        else if(loanid == -2) printf("Book not available. Try again later or reserve (not implemented in Phase1).\n");
                        else printf("Borrow failed.\n");
                    } else if(sc == 3){
                        printf("Enter loan id to return: ");
                        if(!fgets(input, sizeof(input), stdin)) break;
                        int lid = atoi(input);
                        if(return_book_by_loan(lid) == 0) printf("Return recorded.\n");
                        else printf("Return failed.\n");
                    } else if(sc == 4){
                        // show loans for user
                        char *s = read_file("data/loans.txt");
                        if(!s) printf("No loans file.\n");
                        else {
                            printf("Loans file content (search your username):\n%s\n", s);
                            free(s);
                        }
                    } else if(sc == 5){
                        run = 0; printf("Logging out...\n");
                    } else {
                        printf("Invalid choice.\n");
                    }
                } // end student loop
            } else if(role == 2){
                int run = 1;
                while(run){
                    show_librarian_menu();
                    if(!fgets(input, sizeof(input), stdin)) { run = 0; break; }
                    int sc = atoi(input);
                    if(sc == 1){
                        char title[256], author[256], copies_s[32];
                        printf("Title: "); if(!fgets(title,sizeof(title),stdin)) break; title[strcspn(title,"\r\n")] = 0;
                        printf("Author: "); if(!fgets(author,sizeof(author),stdin)) break; author[strcspn(author,"\r\n")] = 0;
                        printf("Copies: "); if(!fgets(copies_s,sizeof(copies_s),stdin)) break;
                        int copies = atoi(copies_s);
                        if(add_book(title, author, copies) == 0) printf("Book added.\n"); else printf("Add failed.\n");
                    } else if(sc == 2){
                        printf("Book id to remove: ");
                        if(!fgets(input,sizeof(input),stdin)) break;
                        int bid = atoi(input);
                        if(remove_book(bid) == 0) printf("Removed.\n"); else printf("Remove failed.\n");
                    } else if(sc == 3){
                        char *b = list_books();
                        printf("Books:\n%s\n", b);
                        free(b);
                    } else if(sc == 4){
                        run = 0; printf("Logging out librarian...\n");
                    } else {
                        printf("Invalid choice.\n");
                    }
                }
            } else if(role == 3){
                int run = 1;
                while(run){
                    show_admin_menu();
                    if(!fgets(input, sizeof(input), stdin)) { run = 0; break; }
                    int sc = atoi(input);
                    if(sc == 1){
                        char *u = list_users();
                        printf("Users:\n%s\n", u);
                        free(u);
                    } else if(sc == 2){
                        char *l = read_file("data/loans.txt");
                        if(!l) printf("No loans.\n"); else { printf("Loans:\n%s\n", l); free(l); }
                    } else if(sc == 3){
                        char *b = list_books();
                        printf("Books:\n%s\n", b);
                        free(b);
                    } else if(sc == 4){
                        run = 0; printf("Admin logging out...\n");
                    } else {
                        printf("Invalid choice.\n");
                    }
                }
            }
        } else if(ch == 2){
            // register
            char username[128], password[128], role_s[8];
            printf("New username: ");
            if(!fgets(username, sizeof(username), stdin)) break;
            username[strcspn(username, "\r\n")] = 0;
            printf("New password: ");
            if(!fgets(password, sizeof(password), stdin)) break;
            password[strcspn(password, "\r\n")] = 0;
            printf("Role (1=student,2=librarian,3=admin): ");
            if(!fgets(role_s, sizeof(role_s), stdin)) break;
            int role = atoi(role_s);
            if(role < 1 || role > 3) { printf("Invalid role.\n"); continue; }
            UserStatus status = register_user(username, password, role);
            printf("%s\n", user_status_message(status));
        } else if(ch == 3){
            printf("Exiting. Bye.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    } // end while
}

