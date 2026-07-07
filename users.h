#ifndef USERS_H
#define USERS_H

// role: 1=student, 2=librarian, 3=admin
int authenticate_user(const char *username, const char *password); // returns role or 0
int register_user(const char *username, const char *password, int role);
char *list_users(); // malloc'ed string, free after use

#endif // USERS_H

