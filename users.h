#ifndef USERS_H
#define USERS_H

#define USERNAME_MAX 50
#define PASSWORD_MAX 128
#define MIN_PASSWORD_LENGTH 8
#define USER_LINE_MAX 256

typedef struct {
    char username[USERNAME_MAX];
    char password[PASSWORD_MAX];
    char line[USER_LINE_MAX];
    int role;
} User;

typedef enum {
    USER_SUCCESS = 0,

    USERNAME_EMPTY,
    USERNAME_EXISTS,
    USERNAME_TOO_LONG,
    USERNAME_INVALID,

    PASSWORD_EMPTY,
    PASSWORD_TOO_SHORT,
    PASSWORD_NO_UPPERCASE,
    PASSWORD_NO_LOWERCASE,
    PASSWORD_NO_DIGIT,
    PASSWORD_NO_SPECIAL,

    INVALID_ROLE,

    STORAGE_ERROR
} UserStatus;

int authenticate_user(const char *username, const char *password);

UserStatus register_user(
    const char *username,
    const char *password,
    int role
);

int authenticate_user(const char *username, const char *password);

UserStatus register_user(
    const char *username,
    const char *password,
    int role
);

const char *user_status_message(UserStatus status);

char *list_users();

#endif